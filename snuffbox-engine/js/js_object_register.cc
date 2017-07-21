#include "js_object_register.h"

#include "../logging/logger.h"
#include "../logging/cvar.h"
#include "../io/content_manager.h"
#include "../core/window.h"

namespace snuffbox
{
	namespace engine
	{
		//class TestObject : JS_OBJECT
		//{
		//public:

		//	JS_NAME_TMPL(TestObject);

		//	JS_FUNCTION_DECL(setValue);
		//	JS_FUNCTION_DECL(getValue);

		//private:

		//	float value_;
		//};

		////-----------------------------------------------------------------------------------------------
		//JS_CONSTRUCTOR(TestObject, JS_BODY({
		//	value_ = 0.0f;
		//}))

		////-----------------------------------------------------------------------------------------------
		//JS_REGISTER_IMPL_TMPL(TestObject, JS_BODY({
		//	JSFunctionRegister funcs[] = {
		//		JS_FUNCTION_REG(setValue),
		//		JS_FUNCTION_REG(getValue),
		//		JS_FUNCTION_REG_END
		//	};

		//	JSFunctionRegister::Register(funcs, obj);
		//}))

		//JS_FUNCTION_IMPL(TestObject, setValue, JS_BODY({
		//	JS_SETUP(TestObject);

		//	if (wrapper.Check("N") == true)
		//	{
		//		self->value_ = wrapper.GetValue<float>(0, 0.0f);
		//	}
		//}))

		//JS_FUNCTION_IMPL(TestObject, getValue, JS_BODY({
		//	JS_SETUP(TestObject);
		//	printf("%f\n\n\n", self->value_);
		//	wrapper.ReturnValue<float>(self->value_);
		//}))

		//-----------------------------------------------------------------------------------------------
		void JSRegister::RegisterSingletons()
		{
			JSObjectRegister<Logger>::RegisterSingleton();
			JSObjectRegister<CVar>::RegisterSingleton();
			JSObjectRegister<ContentManager>::RegisterSingleton();
			JSObjectRegister<Window>::RegisterSingleton();
		}

		//-----------------------------------------------------------------------------------------------
		void JSRegister::RegisterConstructables()
		{
			//JSObjectRegister<TestObject>::Register();
		}

		//-----------------------------------------------------------------------------------------------
		void JSRegister::Register()
		{
			RegisterSingletons();
			RegisterConstructables();
		}
	}
}
