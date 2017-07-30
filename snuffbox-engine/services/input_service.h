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

			virtual bool GetKeyboardPressed(KeyCodes::KeyCode ch) const;
			virtual bool GetKeyboardDown(KeyCodes::KeyCode ch) const;
			virtual bool GetKeyboardReleased(KeyCodes::KeyCode ch) const;
			virtual KeyCodes::KeyCode GetLastKeyboardPressed() const;
			virtual KeyCodes::KeyCode GetLastKeyboardDown() const;
			virtual KeyCodes::KeyCode GetLastKeyboardReleased() const;

			virtual void GetMousePosition(int* px, int* py) const;
			virtual void GetMouseMovement(int* px, int* py) const;
			virtual int GetMouseWheelDelta() const;
			virtual bool GetMousePressed(KeyCodes::KeyCode button) const;
			virtual bool GetMouseDown(KeyCodes::KeyCode button) const;
			virtual bool GetMouseReleased(KeyCodes::KeyCode button) const;
			virtual KeyCodes::KeyCode GetLastMousePressed() const;
			virtual KeyCodes::KeyCode GetLastMouseDown() const;
			virtual KeyCodes::KeyCode GetLastMouseReleased() const;

			virtual bool GetControllerConnected(int id) const;
			virtual void SetControllerConnectedCallback(ControllerButtons::ConnectedCallback cb);
			virtual void SetControllerDisconnectedCallback(ControllerButtons::DisconnectedCallback cb);
			virtual float GetControllerAxis(int id, ControllerButtons::Axes axis) const;
			virtual bool GetControllerPressed(int id, KeyCodes::KeyCode button) const;
			virtual bool GetControllerDown(int id, KeyCodes::KeyCode button) const;
			virtual bool GetControllerReleased(int id, KeyCodes::KeyCode button) const;
			virtual KeyCodes::KeyCode GetLastControllerPressed() const;
			virtual KeyCodes::KeyCode GetLastControllerDown() const;
			virtual KeyCodes::KeyCode GetLastControllerReleased() const;
		};
	}
}