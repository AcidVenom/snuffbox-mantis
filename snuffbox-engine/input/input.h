#pragma once

#include "../js/js_defines.h"
#include "../services/input_service.h"

#include "keyboard.h"

namespace snuffbox
{
	namespace engine
	{
		class SnuffboxApp;
		class Window;
		class Allocator;

		/**
		* @class snuffbox::engine::Input : [JSObject] public snuffbox::engine::InputService
		* @brief The input service to handle keyboard, mouse and controller input with
		* @author Daniel Konings
		*/
		class Input : JS_OBJECT_MULTI public InputService
		{
			friend class SnuffboxApp;
			friend class Window;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor
			*/
			Input();

			/**
			* @brief Checks if any keyboard key has a specified state
			* @param[in] state (snuffbox::engine::Keyboard::KeyState) The state to check for
			* @return (bool) Was any key found with the specified state?
			*/
			bool KeyboardAny(Keyboard::KeyState state) const;

			/**
			* @brief Updates all queues of every input type
			*/
			void Update();

		public:

			/**
			* @see snuffbox::engine::InputService::KeyboardPressed
			*/
			bool KeyboardPressed(KeyCodes::KeyCode ch) const override;

			/**
			* @see snuffbox::engine::InputService::KeyboardDown
			*/
			bool KeyboardDown(KeyCodes::KeyCode ch) const override;

			/**
			* @see snuffbox::engine::InputService::KeyboardReleased
			*/
			bool KeyboardReleased(KeyCodes::KeyCode ch) const override;

			/**
			* @see snuffbox::engine::InputService::LastKeyboardPressed
			*/
			KeyCodes::KeyCode LastKeyboardPressed() const override;

			/**
			* @see snuffbox::engine::InputService::LastKeyboardDown
			*/
			KeyCodes::KeyCode LastKeyboardDown() const override;

			/**
			* @see snuffbox::engine::InputService::LastKeyboardReleased
			*/
			KeyCodes::KeyCode LastKeyboardReleased() const override;

		protected:

			/**
			* @see http://www.glfw.org/docs/latest/input_guide.html
			*/
			static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		private:

			Keyboard keyboard_; //!< The keyboard
			InputType last_type_; //!< The last input type

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