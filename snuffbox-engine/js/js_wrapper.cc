#include "js_wrapper.h"

#include "../services/log_service.h"

using namespace v8;

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		JSWrapper::JSWrapper(const FunctionCallbackInfo<Value>& args) :
			args_(args),
			error_checks_(true),
			lock_(JSStateWrapper::Instance()->isolate())
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		JSWrapper::Types JSWrapper::TypeOf(const Local<Value>& value)
		{
			if (value->IsFunction())
			{
				return Types::kFunction;
			}
			else if (value->IsArray())
			{
				return Types::kArray;
			}
			else if (value->IsObject())
			{
				return Types::kObject;
			}
			else if (value->IsNumber())
			{
				return Types::kNumber;
			}
			else if (value->IsNull())
			{
				return Types::kNull;
			}
			else if (value->IsBoolean())
			{
				return Types::kBoolean;
			}
			else if (value->IsUndefined())
			{
				return Types::kUndefined;
			}
			else if (value->IsString())
			{
				return Types::kString;
			}
			else
			{
				return Types::kUnknown;
			}
		}

		//-----------------------------------------------------------------------------------------------
		engine::String JSWrapper::TypeToString(Types type)
		{
			switch (type)
			{
			case Types::kVoid: 
				return "void";
			case Types::kString: 
				return "string";
			case Types::kFunction: 
				return "function";
			case Types::kArray: 
				return "array";
			case Types::kObject: 
				return "object";
			case Types::kNumber: 
				return "number";
			case Types::kNull: 
				return "null";
			case Types::kBoolean: 
				return "boolean";
			case Types::kUndefined: 
				return "undefined";
			case Types::kUnknown: 
				return "unknown";
			default: 
				return "unknown";
			}
		}

		//-----------------------------------------------------------------------------------------------
		Local<v8::String> JSWrapper::CreateString(const engine::String& utf8)
		{
			Isolate* isolate = JSStateWrapper::Instance()->isolate();
			return v8::String::NewFromUtf8(isolate, utf8.c_str(), NewStringType::kNormal).ToLocalChecked();
		}

		//-----------------------------------------------------------------------------------------------
		Local<Object> JSWrapper::CreateObject()
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			Isolate* isolate = wrapper->isolate();

			Local<ObjectTemplate> t = ObjectTemplate::New(isolate);
			return t->NewInstance(wrapper->Context()).ToLocalChecked();
		}

		//-----------------------------------------------------------------------------------------------
		void JSWrapper::SetPointer(const v8::Local<v8::Object>& obj, void* ptr)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			Isolate* isolate = wrapper->isolate();

			obj->SetPrivate(wrapper->Context(),
				v8::Private::ForApi(isolate, CreateString("__ptr")),
				v8::External::New(isolate, ptr));
		}

		//-----------------------------------------------------------------------------------------------
		void JSWrapper::RegisterGlobal(const char* name, const Local<Value>& val, bool in_namespace)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			if (in_namespace == false)
			{
				wrapper->RegisterGlobal(name, val);
				return;
			}

			SetObjectValue<v8::Local<v8::Value>>(wrapper->Namespace(), name, val);
		}

		//-----------------------------------------------------------------------------------------------
		bool JSWrapper::Check(const char* format, bool force)
		{
#ifdef SNUFF_RELEASE
			if (force == false)
			{
				return true;
			}
#endif
			size_t len = strlen(format);
			if (len == 0)
			{
				return true;
			}

			char f = 'V';
			Types expected, got;
			for (unsigned int i = 0; i < static_cast<unsigned int>(len); ++i)
			{
				f = tolower(format[i]);

				switch (f)
				{
				case 'v':
					expected = Types::kVoid;
					break;
				case 'n':
					expected = Types::kNumber;
					break;
				case 'b':
					expected = Types::kBoolean;
					break;
				case 's':
					expected = Types::kString;
					break;
				case 'o':
					expected = Types::kObject;
					break;
				case 'a':
					expected = Types::kArray;
					break;
				case 'f':
					expected = Types::kFunction;
					break;
				}

				if (expected == Types::kVoid)
				{
					return true;
				}

				got = TypeOf(args_[i]);
				if (got != expected)
				{
					Error(expected, got, i);
					return false;
				}
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void JSWrapper::Error(Types expected, Types got, int arg)
		{
			if (error_checks_ == false)
			{
				return;
			}

			Isolate* isolate = args_.GetIsolate();
            Local<Context> ctx = JSStateWrapper::Instance()->Context();

			Services::Get<LogService>().Log(console::LogSeverity::kError, "\n\n({0}.{1}) Expected '{2}' but got '{3}' for argument {4}\n\t", 
				*v8::String::Utf8Value(args_.This()->ToString(ctx).ToLocalChecked()),
				*v8::String::Utf8Value(args_.Callee()->GetName()->ToString(ctx).ToLocalChecked()),
				TypeToString(expected),
				TypeToString(got),
				std::to_string(arg + 1).c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void JSWrapper::set_error_checks(bool value)
		{
			error_checks_ = value;
		}

		//-----------------------------------------------------------------------------------------------
		JSWrapper::~JSWrapper()
		{
			
		}
	}
}
