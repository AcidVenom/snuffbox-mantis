#include "js_object_register.h"

#include "../logging/logger.h"
#include "../logging/cvar.h"

#include "../io/file.h"
#include "../io/content_manager.h"

#include "../core/window.h"
#include "../core/timer.h"

#include "../input/input.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void JSRegister::RegisterSingletons(const v8::Local<v8::Object>& ns)
		{
			JSObjectRegister<Logger>::RegisterSingleton(ns);
			JSObjectRegister<CVar>::RegisterSingleton(ns);
			JSObjectRegister<ContentManager>::RegisterSingleton(ns);
			JSObjectRegister<Window>::RegisterSingleton(ns);
			JSObjectRegister<Input>::RegisterSingleton(ns);
		}

		//-----------------------------------------------------------------------------------------------
		void JSRegister::RegisterConstructables(const v8::Local<v8::Object>& ns)
		{
			JSObjectRegister<File>::Register(ns);
			JSObjectRegister<Timer>::Register(ns);
		}

		//-----------------------------------------------------------------------------------------------
		void JSRegister::Register()
		{
			v8::Local<v8::Object> ns = JSStateWrapper::Instance()->Namespace();
			RegisterSingletons(ns);
			RegisterConstructables(ns);
		}
	}
}
