#include <libplatform/libplatform.h>
#include <algorithm>

#include "js_state_wrapper.h"
#include "js_defines.h"
#include "js_function_register.h"
#include "js_object_register.h"

#include "../services/log_service.h"
#include "../services/content_service.h"
#include "../services/window_service.h"

#include "../io/script.h"

using namespace v8;

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		JSStateWrapper* JSStateWrapper::instance_ = nullptr;

		//-----------------------------------------------------------------------------------------------
		const unsigned int JSStateWrapper::STACK_LIMIT_ = 1024 * 1024 * 2;

		//-----------------------------------------------------------------------------------------------
		JSStateWrapper::JSStateWrapper(Allocator& allocator) :
            allocator_(allocator),
			isolate_(nullptr),
			platform_(nullptr)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void JSStateWrapper::Initialise()
		{
			assert(instance_ == nullptr);

			LogService& log = Services::Get<LogService>();

			V8::Initialize();
			platform_ = platform::CreateDefaultPlatform();
			V8::InitializePlatform(platform_);

			Isolate::CreateParams params;
            params.array_buffer_allocator = &allocator_;
			isolate_ = Isolate::New(params);
			isolate_->Enter();

			HandleScope scope(isolate_);

            Local<ObjectTemplate> global = CreateGlobal();
			global_.Reset(isolate_, global);

            Local<v8::Context> context = CreateContext(global);
			context_.Reset(isolate_, context);

			context->Enter();

			instance_ = this;
			RegisterCommon();
            JSRegister::Register();

			log.Log(console::LogSeverity::kSuccess, "Successfully initialised V8");
		}

		//-----------------------------------------------------------------------------------------------
        Local<ObjectTemplate> JSStateWrapper::CreateGlobal() const
		{
			return ObjectTemplate::New(isolate_);
		}

		//-----------------------------------------------------------------------------------------------
        Local<Context> JSStateWrapper::CreateContext(const Local<ObjectTemplate>& global) const
		{
			return Context::New(isolate_, nullptr, global);
		}

		//-----------------------------------------------------------------------------------------------
		bool JSStateWrapper::GetException(TryCatch* try_catch, engine::String* out) const
		{
			HandleScope handle_scope(isolate_);
			v8::String::Utf8Value exception(try_catch->Exception());
            Local<Message> message = try_catch->Message();

			engine::String error = "";

			if (!message.IsEmpty())
			{
                v8::String::Utf8Value sourceline(message->GetSourceLine(Context()).ToLocalChecked());
				error += "\n\n";

				engine::String srcline = *sourceline;

				char* it = std::remove_if(std::begin(srcline), std::end(srcline), [](char c)
				{
					return (c == '\t');
				});

				srcline.erase(it, std::end(srcline));

				error += srcline;
				error += "\n";

                int start = message->GetStartColumn(Context()).ToChecked();

				for (int i = 0; i < start; ++i)
				{
					error += " ";
				}

				int end = message->GetEndColumn();

				for (int i = start; i < end; ++i)
				{
					error += "^";
				}

                v8::String::Utf8Value stack_trace(try_catch->StackTrace(Context()).ToLocalChecked());

				error += "\n\t";
				error += *stack_trace;
				error += "\n";

				if (stack_trace.length() > 0)
				{
					if (out != nullptr)
					{
						*out = error;
					}

					return true;
				}
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		engine::String JSStateWrapper::StackTrace(const unsigned int& max) const
        {
			HandleScope scope(isolate_);
			engine::String error;
			Local<v8::StackTrace> stack = StackTrace::CurrentStackTrace(isolate_, max);

			Local<StackFrame> frame;
			for (int i = 0; i < stack->GetFrameCount(); ++i)
			{
				frame = stack->GetFrame(i);
				error += "\n\t";
				error += "at " + engine::String(*v8::String::Utf8Value(frame->GetFunctionName())) +
					" (" + engine::String(*v8::String::Utf8Value(frame->GetScriptName())) +
					":" + std::to_string(frame->GetLineNumber()).c_str() + ":" + std::to_string(frame->GetColumn()).c_str() + ")";
			}

			error += "\n";

			return error;
		}

		//-----------------------------------------------------------------------------------------------
        void JSStateWrapper::RegisterGlobal(const char* name, const Local<Value>& value)
		{
			Local<Object> global = Global();
            global->Set(Context(),
                        v8::String::NewFromUtf8(isolate_, name, v8::NewStringType::kNormal).ToLocalChecked(),
                        value);
		}

		//-----------------------------------------------------------------------------------------------
		void JSStateWrapper::CollectGarbage()
		{
			LogService& log = Services::Get<LogService>();

			log.Log(console::LogSeverity::kDebug, "Collecting all JavaScript garbage");

			HandleScope scope(isolate_);
			Local<Object> g = Global();
            Local<v8::Context> ctx = Context();

            for (unsigned int i = 0; i < g->GetPropertyNames(ctx).ToLocalChecked()->Length(); ++i)
			{
                g->Set(ctx, g->GetPropertyNames(ctx).ToLocalChecked()->Get(ctx, i).ToLocalChecked(), v8::Undefined(isolate_));
			}

			log.Log(console::LogSeverity::kDebug, "Collected all JavaScript garbage");
		}

		//-----------------------------------------------------------------------------------------------
		void JSStateWrapper::Dispose()
		{
			LogService& log = Services::Get<LogService>();

			isolate_->LowMemoryNotification();
			isolate_->Exit();
			isolate_->Dispose();

			V8::Dispose();
			V8::ShutdownPlatform();

			log.Log(console::LogSeverity::kSuccess, "Successfully cleaned up V8");
		}

		//-----------------------------------------------------------------------------------------------
		void JSStateWrapper::Shutdown()
		{
			CollectGarbage();
			Dispose();

			instance_ = nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		Local<Object> JSStateWrapper::Global() const
		{
			return Local<v8::Context>::New(isolate_, context_)->Global();
		}

		//-----------------------------------------------------------------------------------------------
		Local<Context> JSStateWrapper::Context() const
		{
			return Local<v8::Context>::New(isolate_, context_);
		}

		//-----------------------------------------------------------------------------------------------
		Isolate* JSStateWrapper::isolate() const
		{
			return isolate_;
		}

		//-----------------------------------------------------------------------------------------------
		bool JSStateWrapper::Run(const engine::String& src, const engine::String& file_name, const bool& echo)
        {
            std::lock_guard<std::mutex> lock(run_mutex_);

            isolate_->Enter();
            isolate_->SetStackLimit(STACK_LIMIT_);
            HandleScope scope(isolate_);

            TryCatch try_catch;

            LogService& log = Services::Get<LogService>();

            Local<v8::Context> ctx = Context();
            Context::Scope context_scope(ctx);

            v8::ScriptOrigin origin(v8::String::NewFromUtf8(isolate_, file_name.c_str(), v8::NewStringType::kNormal).ToLocalChecked());
            v8::ScriptCompiler::Source s(v8::String::NewFromUtf8(isolate_, src.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), origin);

            Local<v8::Script> script;
            bool compiled = v8::ScriptCompiler::Compile(ctx, &s).ToLocal(&script);

            Local<Value> result;

            if (compiled == false || script->Run(ctx).ToLocal(&result) == false)
            {
                engine::String error;
                bool has_error = GetException(&try_catch, &error);

                if (has_error == true)
                {
                    log.Log(console::LogSeverity::kError, "{0}", error);
                }

                isolate_->Exit();
                return false;
            }
			
			if (echo == true && Services::Get<WindowService>().Closed() == false)
			{
                log.Log(console::LogSeverity::kDebug, "{0}", *v8::String::Utf8Value(result->ToString(ctx).ToLocalChecked()));
			}

            isolate_->Exit();
			return true;
		}

		//-----------------------------------------------------------------------------------------------
		JSStateWrapper* JSStateWrapper::Instance()
		{
			assert(instance_ != nullptr);
			return instance_;
		}

		//-----------------------------------------------------------------------------------------------
		void JSStateWrapper::RegisterCommon()
		{
			JSFunctionRegister funcs[] = {
				JS_FUNCTION_REG(require),
				JS_FUNCTION_REG(assert),
				JS_FUNCTION_REG_END
			};

			JSFunctionRegister::Register(funcs);
		}

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(JSStateWrapper, require, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				engine::String path = wrapper.GetValue<engine::String>(0, "");
				Services::Get<ContentService>().Load<engine::Script>(path);
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(JSStateWrapper, assert, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("BS") == true)
			{
				Services::Get<LogService>().Assert(wrapper.GetValue<bool>(0, true), wrapper.GetValue<engine::String>(1, ""));
			}
		}));
	}
}
