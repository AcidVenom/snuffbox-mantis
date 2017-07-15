#include "js_function_register.h"
#include "js_state_wrapper.h"

using namespace v8;

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void JSFunctionRegister::Register(JSFunctionRegister* funcs, const Handle<ObjectTemplate>& obj)
		{
			if (funcs == nullptr || funcs[0].name == nullptr)
			{
				return;
			}

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			Isolate* isolate = wrapper->isolate();

			int current = -1;
			while(funcs[++current].name != nullptr)
			{
				Local<FunctionTemplate> func = FunctionTemplate::New(isolate, funcs[current].function);
				obj->Set(isolate, funcs[current].name, func);
			}
		}

		//-----------------------------------------------------------------------------------------------
		void JSFunctionRegister::Register(JSFunctionRegister* funcs, const Handle<Object>& obj)
		{
			if (funcs == nullptr || funcs[0].name == nullptr)
			{
				return;
			}

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			Isolate* isolate = wrapper->isolate();

			int current = -1;
			while (funcs[++current].name != nullptr)
			{
				Local<Function> func = Function::New(isolate, funcs[current].function);
				func->SetName(v8::String::NewFromUtf8(isolate, funcs[current].name));
				obj->Set(v8::String::NewFromUtf8(isolate, funcs[current].name), func);
			}
		}

		//-----------------------------------------------------------------------------------------------
		void JSFunctionRegister::Register(JSFunctionRegister* funcs)
		{
			if (funcs == nullptr || funcs[0].name == nullptr)
			{
				return;
			}

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			Isolate* isolate = wrapper->isolate();

			Local<Object> global = wrapper->Global();

			int current = -1;
			while (funcs[++current].name != nullptr)
			{
				wrapper->RegisterGlobal(funcs[current].name, Function::New(isolate, funcs[current].function));
			}
		}
	}
}