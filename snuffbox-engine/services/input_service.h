#pragma once

#include "service.h"
#include "services.h"

#include "../input/keycodes.h"
#include "../input/buttons.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::InputService : public snuffbox::engine::Service<snuffbox::engine::ServiceIDs::kInputService>
		* @brief The input service to retrieve input data from the keyboard, mouse and controller with
		* @author Daniel Konings
		*/
		class InputService : public Service<ServiceIDs::kInputService>
		{
			friend class Services;

		public:

			/**
			* @brief Used for the last input type received from the user
			*/
			enum struct InputType
			{
				kKeyboard, //!< The keyboard input type
				kController //!< The controller input type
			};

		protected:

			/**
			* @brief Default constructor, constructs a null-service
			*/
			InputService();

		public:

			/**
			* @brief Delete copy constructor
			*/
			InputService(const InputService& other) = delete;

			/**
			* @brief Delete assignment operator
			*/
			InputService operator=(const InputService& other) = delete;

			/**
			* @brief Retrieves the pressed state for a keyboard keycode
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the key pressed this frame?
			*/
			virtual bool GetKeyboardPressed(KeyCodes::KeyCode ch) const;

			/**
			* @brief Retrieves the down state for a keyboard keycode
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the key down this frame?
			*/
			virtual bool GetKeyboardDown(KeyCodes::KeyCode ch) const;

			/**
			* @brief Retrieves the released state for a keyboard keycode
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the key released this frame?
			*/
			virtual bool GetKeyboardReleased(KeyCodes::KeyCode ch) const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last key that was pressed this frame
			*/
			virtual KeyCodes::KeyCode GetLastKeyboardPressed() const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last key that was down this frame
			*/
			virtual KeyCodes::KeyCode GetLastKeyboardDown() const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last key that was released this frame
			*/
			virtual KeyCodes::KeyCode GetLastKeyboardReleased() const;

			/**
			* @brief Retrieves the current mouse position
			* @param[out] px (int*) The x position within the client area, in pixels
			* @param[out] py (int*) The y position within the client area, in pixels
			*/
			virtual void GetMousePosition(int* px, int* py) const;

			/**
			* @brief Retrieves the current mouse movement delta
			* @param[out] px (int*) The x delta in pixels
			* @param[out] py (int*) The y delta in pixels
			*/
			virtual void GetMouseMovement(int* px, int* py) const;

			/**
			* @return (int) The delta of the scroll wheel
			*/
			virtual int GetMouseWheelDelta() const;

			/**
			* @brief Retrieves the pressed state for a mouse keycode
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the mouse button pressed this frame?
			*/
			virtual bool GetMousePressed(KeyCodes::KeyCode button) const;

			/**
			* @brief Retrieves the down state for a mouse keycode
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the mouse button down this frame?
			*/
			virtual bool GetMouseDown(KeyCodes::KeyCode button) const;

			/**
			* @brief Retrieves the released state for a mouse keycode
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the mouse button released this frame?
			*/
			virtual bool GetMouseReleased(KeyCodes::KeyCode button) const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last mouse button that was pressed this frame
			*/
			virtual KeyCodes::KeyCode GetLastMousePressed() const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last mouse button that was down this frame
			*/
			virtual KeyCodes::KeyCode GetLastMouseDown() const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last mouse button that was released this frame
			*/
			virtual KeyCodes::KeyCode GetLastMouseReleased() const;

			/**
			* @brief Retrieves if a specified controller is connected
			* @param[in] id (int) The ID to check for
			* @return (bool) Is the controller of the specified ID connected?
			*/
			virtual bool GetControllerConnected(int id) const;

			/**
			* @return (unsigned int) The number of connected controllers
			*/
			virtual unsigned int ConnectedControllers() const;

			/**
			* @brief Sets a callback for when a controller is connected to the system
			* @param[in] cb (snuffbox::engine::ControllerButtons::ConnectedCallback) The callback to call once a controller connects
			*/
			virtual void SetControllerConnectedCallback(ControllerButtons::ConnectedCallback cb);

			/**
			* @brief Sets a callback for when a controller is disconnected from the system
			* @param[in] cb (snuffbox::engine::ControllerButtons::DisconnectedCallback) The callback to call once a controller disconnects
			*/
			virtual void SetControllerDisconnectedCallback(ControllerButtons::DisconnectedCallback cb);

			/**
			* @brief Retrieves the floating point value between -1 and 1 of a controller axis
			* @param[in] id (int) The controller ID to check the axix for
			* @param[in] axis (snuffbox::engine::ControllerButtons::Axes) The axis to check for
			* @return (float) The axis represented as a floating point value in the -1 < 0 < 1 range
			*/
			virtual float GetControllerAxis(int id, ControllerButtons::Axes axis) const;

			/**
			* @brief Retrieves the pressed state for a controller button
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the controller button pressed this frame?
			*/
			virtual bool GetControllerPressed(int id, KeyCodes::KeyCode button) const;

			/**
			* @brief Retrieves the down state for a controller button
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the controller button down this frame?
			*/
			virtual bool GetControllerDown(int id, KeyCodes::KeyCode button) const;

			/**
			* @brief Retrieves the released state for a controller button
			* @param[in] ch (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was the controller button released this frame?
			*/
			virtual bool GetControllerReleased(int id, KeyCodes::KeyCode button) const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last controller button that was pressed this frame
			*/
			virtual KeyCodes::KeyCode GetLastControllerPressed() const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last controller button that was down this frame
			*/
			virtual KeyCodes::KeyCode GetLastControllerDown() const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last controller button that was released this frame
			*/
			virtual KeyCodes::KeyCode GetLastControllerReleased() const;
		};
	}
}