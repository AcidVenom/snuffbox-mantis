#include "js_function_register.h"
#include "js_state_wrapper.h"
#include "js_wrapper.h"

using namespace v8;

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
        void JSFunctionRegister::Register(JSFunctionRegister* funcs, const Local<ObjectTemplate>& obj)
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
        void JSFunctionRegister::Register(JSFunctionRegister* funcs, const Local<Object>& obj)
		{
			if (funcs == nullptr || funcs[0].name == nullptr)
			{
				return;
			}

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			Isolate* isolate = wrapper->isolate();
			Local<Context> ctx = wrapper->Context();

			Local<Function> func;
			Local<v8::String> name;

			int current = -1;
			while (funcs[++current].name != nullptr)
			{
                func = Function::New(ctx, funcs[current].function).ToLocalChecked();
				name = JSWrapper::CreateString(funcs[current].name);

                func->SetName(name);
                obj->Set(ctx, name, func);
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

			Local<Object> global = wrapper->Global();

			int current = -1;
			while (funcs[++current].name != nullptr)
			{
                wrapper->RegisterGlobal(funcs[current].name, Function::New(wrapper->Context(), funcs[current].function).ToLocalChecked());
			}
		}
	}
}
