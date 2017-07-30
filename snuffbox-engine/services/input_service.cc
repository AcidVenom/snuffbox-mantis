#include "input_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		InputService::InputService()
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetKeyboardPressed(KeyCodes::KeyCode ch) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetKeyboardDown(KeyCodes::KeyCode ch) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetKeyboardReleased(KeyCodes::KeyCode ch) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastKeyboardPressed() const
		{
			return KeyCodes::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastKeyboardDown() const
		{
			return KeyCodes::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastKeyboardReleased() const
		{
			return KeyCodes::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		void InputService::GetMousePosition(int* px, int* py) const
		{
			if (px != nullptr)
			{
				*px = 0;
			}

			if (py != nullptr)
			{
				*py = 0;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void InputService::GetMouseMovement(int* px, int* py) const
		{
			if (px != nullptr)
			{
				*px = 0;
			}

			if (py != nullptr)
			{
				*py = 0;
			}
		}

		//-----------------------------------------------------------------------------------------------
		int InputService::GetMouseWheelDelta() const
		{
			return 0;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetMousePressed(KeyCodes::KeyCode button) const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetMouseDown(KeyCodes::KeyCode button) const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetMouseReleased(KeyCodes::KeyCode button) const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastMousePressed() const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastMouseDown() const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastMouseReleased() const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetControllerConnected(int id) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int InputService::ConnectedControllers() const
		{
			return 0;
		}

		//-----------------------------------------------------------------------------------------------
		void InputService::SetControllerConnectedCallback(ControllerButtons::ConnectedCallback cb)
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		void InputService::SetControllerDisconnectedCallback(ControllerButtons::DisconnectedCallback cb)
		{

		}

		//-----------------------------------------------------------------------------------------------
		float InputService::GetControllerAxis(int id, ControllerButtons::Axes axis) const
		{
			return 0.0f;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetControllerPressed(int id, KeyCodes::KeyCode button) const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetControllerDown(int id, KeyCodes::KeyCode button) const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::GetControllerReleased(int id, KeyCodes::KeyCode button) const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastControllerPressed() const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastControllerDown() const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::GetLastControllerReleased() const
		{
			return ControllerButtons::kNone;
		}
	}
}