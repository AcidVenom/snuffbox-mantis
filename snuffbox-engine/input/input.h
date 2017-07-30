#pragma once

#include "../js/js_defines.h"
#include "../services/input_service.h"

namespace snuffbox
{
	namespace engine
	{
		class Input : JS_OBJECT_MULTI public InputService
		{
			

		public:

			JS_NAME_SINGLE(Input);

			JS_FUNCTION_DECL(keyboardPressed);
			JS_FUNCTION_DECL(keyboardDown);
			JS_FUNCTION_DECL(keyboardReleased);
			JS_FUNCTION_DECL(lastKeyboardPressed);
			JS_FUNCTION_DECL(lastKeyboardDown);
			JS_FUNCTION_DECL(lastKeyboardReleased);
			JS_FUNCTION_DECL(mousePosition);
			JS_FUNCTION_DECL(mouseMovement);
			JS_FUNCTION_DECL(mouseWheelDelta);
			JS_FUNCTION_DECL(mousePressed);
			JS_FUNCTION_DECL(mouseDown);
			JS_FUNCTION_DECL(mouseReleased);
			JS_FUNCTION_DECL(lastMousePressed);
			JS_FUNCTION_DECL(lastMouseDown);
			JS_FUNCTION_DECL(lastMouseReleased);
			JS_FUNCTION_DECL(isControllerConnected);
			JS_FUNCTION_DECL(connectedControllers);
			JS_FUNCTION_DECL(controllerAxis);
			JS_FUNCTION_DECL(controllerPressed);
			JS_FUNCTION_DECL(controllerDown);
			JS_FUNCTION_DECL(controllerReleased);
			JS_FUNCTION_DECL(lastControllerPressed);
			JS_FUNCTION_DECL(lastControllerDown);
			JS_FUNCTION_DECL(lastControllerReleased);
		};
	}
}