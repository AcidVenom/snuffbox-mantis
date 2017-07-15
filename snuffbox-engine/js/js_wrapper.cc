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
			error_checks_(true)
		{
			JSStateWrapper::Instance()->isolate()->Enter();
		}

		//-----------------------------------------------------------------------------------------------
		Handle<Object> JSWrapper::CreateObject()
		{
			Isolate* isolate = JSStateWrapper::Instance()->isolate();
			return Object::New(isolate);
		}

		//-----------------------------------------------------------------------------------------------
		Handle<Array> JSWrapper::CreateArray()
		{
			Isolate* isolate = JSStateWrapper::Instance()->isolate();
			return Array::New(isolate);
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
		engine::String JSWrapper::TypeToString(const Types& type)
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
		bool JSWrapper::Check(const engine::String& format)
		{
			if (format.size() == 0)
			{
				return true;
			}

			char f = 'V';
			Types expected, got;
			for (int i = 0; i < format.size(); ++i)
			{
				f = tolower(format.at(i));

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
		void JSWrapper::Error(const Types& expected, const Types& got, const int& arg)
		{
			if (error_checks_ == false)
			{
				return;
			}

			Isolate* isolate = JSStateWrapper::Instance()->isolate();
			engine::String error = "(";

			error += *v8::String::Utf8Value(args_.This()->ToString());
			error += ".";
			error += *v8::String::Utf8Value(args_.Callee()->GetName()->ToString());
			error += ") ";

			error += "Expected '" + TypeToString(expected) + "', but got '" + TypeToString(got) + "' for argument " + std::to_string(arg + 1).c_str() + "\n\t";

			Services::Get<LogService>().Log(console::LogSeverity::kError, "{0}", error.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void JSWrapper::set_error_checks(const bool& value)
		{
			error_checks_ = value;
		}

		//-----------------------------------------------------------------------------------------------
		JSWrapper::~JSWrapper()
		{
			JSStateWrapper::Instance()->isolate()->Exit();
		}
	}
}