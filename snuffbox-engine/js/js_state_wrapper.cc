#include <libplatform/libplatform.h>
#include <algorithm>

#include "js_state_wrapper.h"
#include "js_defines.h"
#include "js_function_register.h"
#include "js_object_register.h"

#include "../services/log_service.h"
#include "../services/content_service.h"

#include "../io/script.h"

using namespace v8;

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		JSStateWrapper* JSStateWrapper::instance_ = nullptr;

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

			V8::InitializeICU();
			V8::Initialize();
			platform_ = platform::CreateDefaultPlatform();
			V8::InitializePlatform(platform_);

			Isolate::CreateParams params;
			params.array_buffer_allocator = &allocator_;
			isolate_ = Isolate::New(params);
			isolate_->Enter();

			HandleScope scope(isolate_);

			Handle<ObjectTemplate> global = CreateGlobal();
			global_.Reset(isolate_, global);

			Handle<v8::Context> context = CreateContext(global);
			context_.Reset(isolate_, context);

			context->Enter();

			instance_ = this;
			RegisterCommon();
			JSRegister::Register();

			isolate_->Exit();

			log.Log(console::LogSeverity::kSuccess, "Successfully initialised V8");
		}

		//-----------------------------------------------------------------------------------------------
		Handle<ObjectTemplate> JSStateWrapper::CreateGlobal() const
		{
			return ObjectTemplate::New(isolate_);
		}

		//-----------------------------------------------------------------------------------------------
		Handle<Context> JSStateWrapper::CreateContext(const Handle<ObjectTemplate>& global) const
		{
			return Context::New(isolate_, nullptr, global);
		}

		//-----------------------------------------------------------------------------------------------
		bool JSStateWrapper::GetException(TryCatch* try_catch, engine::String* out) const
		{
			HandleScope handle_scope(isolate_);
			v8::String::Utf8Value exception(try_catch->Exception());
			Handle<Message> message = try_catch->Message();

			engine::String error = "";

			if (!message.IsEmpty())
			{
				v8::String::Utf8Value sourceline(message->GetSourceLine());
				error += "\n\n";

				engine::String srcline = *sourceline;

				char* it = std::remove_if(std::begin(srcline), std::end(srcline), [](char c)
				{
					return (c == '\t');
				});

				srcline.erase(it, std::end(srcline));

				error += srcline;
				error += "\n";

				int start = message->GetStartColumn();

				for (int i = 0; i < start; ++i)
				{
					error += " ";
				}

				int end = message->GetEndColumn();

				for (int i = start; i < end; ++i)
				{
					error += "^";
				}

				v8::String::Utf8Value stack_trace(try_catch->StackTrace());

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
		void JSStateWrapper::RegisterGlobal(const char* name, const Handle<Value>& value)
		{
			Local<Object> global = Global();
			global->Set(v8::String::NewFromUtf8(isolate_, name), value);
		}

		//-----------------------------------------------------------------------------------------------
		void JSStateWrapper::CollectGarbage()
		{
			LogService& log = Services::Get<LogService>();

			log.Log(console::LogSeverity::kDebug, "Collecting all JavaScript garbage");

			HandleScope scope(isolate_);
			Local<Object> g = Global();

			for (unsigned int i = 0; i < g->GetPropertyNames()->Length(); ++i)
			{
				g->Set(g->GetPropertyNames()->Get(i), v8::Undefined(isolate_));
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
			std::lock_guard<std::recursive_mutex> lock(run_mutex_);

			isolate_->Enter();
			HandleScope scope(isolate_);

			LogService& log = Services::Get<LogService>();

			Local<v8::Context> context = Local<v8::Context>::New(isolate_, context_);
			Context::Scope context_scope(context);

			TryCatch try_catch;

			Local<v8::Script> script = v8::Script::Compile(v8::String::NewFromUtf8(isolate_, src.c_str()), v8::String::NewFromUtf8(isolate_, file_name.c_str()));
			Local<Value> result = script->Run();

			if (result.IsEmpty() == true)
			{
				engine::String error;
				bool has_error = GetException(&try_catch, &error);

				if (has_error == true)
				{
					log.Log(console::LogSeverity::kError, "{0}", error.c_str());
				}

				isolate_->Exit();
				return false;
			}
			
			if (echo == true)
			{
				log.Log(console::LogSeverity::kDebug, "{0}", *v8::String::Utf8Value(result->ToString()));
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
