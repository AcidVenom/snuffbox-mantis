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
		bool InputService::KeyboardPressed(KeyCodes::KeyCode ch) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::KeyboardDown(KeyCodes::KeyCode ch) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::KeyboardReleased(KeyCodes::KeyCode ch) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastKeyboardPressed() const
		{
			return KeyCodes::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastKeyboardDown() const
		{
			return KeyCodes::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastKeyboardReleased() const
		{
			return KeyCodes::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		void InputService::MousePosition(int* px, int* py) const
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
		void InputService::MouseMovement(int* px, int* py) const
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
		int InputService::MouseWheelDelta() const
		{
			return 0;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::MousePressed(KeyCodes::KeyCode button) const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::MouseDown(KeyCodes::KeyCode button) const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::MouseReleased(KeyCodes::KeyCode button) const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastMousePressed() const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastMouseDown() const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastMouseReleased() const
		{
			return MouseButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::IsControllerConnected(int id) const
		{
			return false;
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int InputService::ConnectedControllers() const
		{
			return 0;
		}

		//-----------------------------------------------------------------------------------------------
		float InputService::ControllerAxis(int id, ControllerButtons::Axes axis) const
		{
			return 0.0f;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::ControllerPressed(int id, KeyCodes::KeyCode button) const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::ControllerDown(int id, KeyCodes::KeyCode button) const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::ControllerReleased(int id, KeyCodes::KeyCode button) const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastControllerPressed() const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastControllerDown() const
		{
			return ControllerButtons::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastControllerReleased() const
		{
			return ControllerButtons::kNone;
		}
	}
}