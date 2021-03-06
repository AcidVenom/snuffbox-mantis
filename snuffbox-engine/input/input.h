#pragma once

#include "../js/js_defines.h"
#include "../services/input_service.h"

#include "keyboard.h"
#include "mouse.h"
#include "controller.h"

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
			friend class Controller;

		protected:

			/**
			* @brief Default constructor
			*/
			Input();

			/**
			* @brief Used to create a key event from a GLFW window event
			* @param[in] key (int) The GLFW key that was received from a callback
			* @param[in] action (int) The GLFW action that was received from a callback
			* @return (snuffbox::engine::KeyQueue::Event) The constructed event
			*/
			static KeyQueue::Event CreateKeyEvent(int key, int action);

			/**
			* @return (snuffbox::engine::Input*) The pointer to the singleton service, used within the callbacks of GLFW
			* @remarks This is done this way, because GLFW still uses C-styled function pointers that do not contain state
			*/
			static Input* Self();

			/**
			* @brief Updates all queues of every input type
			*/
			void Update();

			/**
			* @brief Handles a specified key queue
			* @param[in] queue (const snuffbox::engine::KeyQueue&) The key queue to handle
			* @param[in] key (snuffbox::engine::KeyCodes::KeyCode) The key code to handle the queue with
			* @param[in] state (snuffbox::engine::KeyQueue::KeyState) The state to check for during handling
			* @return (bool) Was the specified key in the specified state?
			*/
			bool HandleKeyQueue(const KeyQueue& queue, KeyCodes::KeyCode key, KeyQueue::KeyState state) const;

			/**
			* @brief Special handle case for controllers, makes sure the controller is connected
			* @param[in] id (int) The ID of the controller
			* @see snuffbox::engine::Input::HandleKeyQueue
			*/
			bool HandleController(int id, KeyCodes::KeyCode key, KeyQueue::KeyState state) const;

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

			/**
			* @see snuffbox::engine::InputService::MousePosition
			*/
			void MousePosition(int* px, int* py) const override;

			/**
			* @see snuffbox::engine::InputService::MouseMovement
			*/
			void MouseMovement(int* mx, int* my) const override;

			/**
			* @see snuffbox::engine::InputService::MouseWheelDelta
			*/
			void MouseWheelDelta(int* sx, int* sy) const override;

			/**
			* @see snuffbox::engine::InputService::MousePressed
			*/
			bool MousePressed(KeyCodes::KeyCode ch) const override;

			/**
			* @see snuffbox::engine::InputService::MouseDown
			*/
			bool MouseDown(KeyCodes::KeyCode ch) const override;

			/**
			* @see snuffbox::engine::InputService::MouseReleased
			*/
			bool MouseReleased(KeyCodes::KeyCode ch) const override;

			/**
			* @see snuffbox::engine::InputService::LastMousePressed
			*/
			KeyCodes::KeyCode LastMousePressed() const override;

			/**
			* @see snuffbox::engine::InputService::LastMouseDown
			*/
			KeyCodes::KeyCode LastMouseDown() const override;

			/**
			* @see snuffbox::engine::InputService::LastMouseReleased
			*/
			KeyCodes::KeyCode LastMouseReleased() const override;

			/**
			* @see snuffbox::engine::InputService::IsControllerConnected
			*/
			bool IsControllerConnected(int id) const override;

			/**
			* @see snuffbox::engine::InputService::ConnectedControllers
			*/
			unsigned int ConnectedControllers() const override;

			/**
			* @see snuffbox::engine::InputService::ConnectedControllers
			*/
			void SetControllerDeadZone(int id, float dz) override;

			/**
			* @see snuffbox::engine::InputService::ControllerAxis
			*/
			float ControllerAxis(int id, ControllerButtons::Axes axis) const override;

			/**
			* @see snuffbox::engine::InputService::ControllerPressed
			*/
			bool ControllerPressed(int id, KeyCodes::KeyCode key) const override;

			/**
			* @see snuffbox::engine::InputService::ControllerDown
			*/
			bool ControllerDown(int id, KeyCodes::KeyCode key) const override;

			/**
			* @see snuffbox::engine::InputService::ControllerReleased
			*/
			bool ControllerReleased(int id, KeyCodes::KeyCode key) const override;

			/**
			* @see snuffbox::engine::InputService::LastControllerPressed
			*/
			KeyCodes::KeyCode LastControllerPressed(int id) const override;

			/**
			* @see snuffbox::engine::InputService::LastControllerDown
			*/
			KeyCodes::KeyCode LastControllerDown(int id) const override;

			/**
			* @see snuffbox::engine::InputService::LastControllerReleased
			*/
			KeyCodes::KeyCode LastControllerReleased(int id) const override;

		protected:

			/**
			* @see http://www.glfw.org/docs/latest/input_guide.html
			*/
			static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			/**
			* @see http://www.glfw.org/docs/latest/input_guide.html
			*/
			static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			/**
			* @see http://www.glfw.org/docs/latest/input_guide.html
			*/
			static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);

			/**
			* @see http://www.glfw.org/docs/latest/input_guide.html
			*/
			static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		private:

			Keyboard keyboard_; //!< The keyboard
			Mouse mouse_; //!< The mouse
			InputType last_type_; //!< The last input type
			Controller controllers_[4]; //!< The controllers, with a maximum of 4

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
			JS_FUNCTION_DECL(setControllerDeadZone);
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