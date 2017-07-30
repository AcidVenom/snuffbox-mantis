#include "input.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_SINGLE(Input, JS_BODY(
		{
			JSFunctionRegister funcs[] = {
				JS_FUNCTION_REG(keyboardPressed),
				JS_FUNCTION_REG(keyboardDown),
				JS_FUNCTION_REG(keyboardReleased),
				JS_FUNCTION_REG(lastKeyboardPressed),
				JS_FUNCTION_REG(lastKeyboardDown),
				JS_FUNCTION_REG(lastKeyboardReleased),
				JS_FUNCTION_REG(mousePosition),
				JS_FUNCTION_REG(mouseMovement),
				JS_FUNCTION_REG(mouseWheelDelta),
				JS_FUNCTION_REG(mousePressed),
				JS_FUNCTION_REG(mouseDown),
				JS_FUNCTION_REG(mouseReleased),
				JS_FUNCTION_REG(lastMousePressed),
				JS_FUNCTION_REG(lastMouseDown),
				JS_FUNCTION_REG(lastMouseReleased),
				JS_FUNCTION_REG(isControllerConnected),
				JS_FUNCTION_REG(connectedControllers),
				JS_FUNCTION_REG(controllerAxis),
				JS_FUNCTION_REG(controllerPressed),
				JS_FUNCTION_REG(controllerDown),
				JS_FUNCTION_REG(controllerReleased),
				JS_FUNCTION_REG(lastControllerPressed),
				JS_FUNCTION_REG(lastControllerDown),
				JS_FUNCTION_REG(lastControllerReleased),
				JS_FUNCTION_REG_END
			};

			v8::Local<v8::Object> key_enum = JSWrapper::CreateObject();
			v8::Local<v8::Object> mb_enum = JSWrapper::CreateObject();
			v8::Local<v8::Object> control_enum = JSWrapper::CreateObject();

			KeyCodes::KeyCodeLoop([=](KeyCodes::KeyCode ch, const char* name) 
			{ 
				JSWrapper::SetObjectValue(key_enum, name, static_cast<unsigned int>(ch)); 
			});

			MouseButtons::MouseButtonsLoop([=](KeyCodes::KeyCode ch, const char* name)
			{ 
				JSWrapper::SetObjectValue(mb_enum, name, static_cast<unsigned int>(ch)); 
			});

			ControllerButtons::ControllerButtonsLoop([=](KeyCodes::KeyCode ch, const char* name) 
			{ 
				JSWrapper::SetObjectValue(control_enum, name, static_cast<unsigned int>(ch)); 
			});

			JSWrapper::RegisterGlobal("Keyboard", key_enum, true);
			JSWrapper::RegisterGlobal("Mouse", mb_enum, true);
			JSWrapper::RegisterGlobal("Controller", control_enum, true);

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardPressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, keyboardReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastKeyboardPressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastKeyboardDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastKeyboardReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mousePosition, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseMovement, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseWheelDelta, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mousePressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, mouseReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMousePressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMouseDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastMouseReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, isControllerConnected, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, connectedControllers, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerAxis, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerPressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, controllerReleased, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerPressed, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerDown, JS_BODY({
		
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Input, lastControllerReleased, JS_BODY({
		
		}));
	}
}