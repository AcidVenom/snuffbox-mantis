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
            return KeyCodesEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastKeyboardDown() const
		{
            return KeyCodesEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastKeyboardReleased() const
		{
            return KeyCodesEnum::kNone;
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
		void InputService::MouseMovement(int* mx, int* my) const
		{
			if (mx != nullptr)
			{
				*mx = 0;
			}

			if (my != nullptr)
			{
				*my = 0;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void InputService::MouseWheelDelta(int* sx, int* sy) const
		{
			if (sx != nullptr)
			{
				*sx = 0;
			}

			if (sy != nullptr)
			{
				*sy = 0;
			}
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::MousePressed(KeyCodes::KeyCode button) const
		{
            return MouseButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::MouseDown(KeyCodes::KeyCode button) const
		{
            return MouseButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::MouseReleased(KeyCodes::KeyCode button) const
		{
            return MouseButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastMousePressed() const
		{
            return MouseButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastMouseDown() const
		{
            return MouseButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastMouseReleased() const
		{
            return MouseButtonsEnum::kNone;
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
		void InputService::SetControllerDeadZone(int id, float dz)
		{

		}

		//-----------------------------------------------------------------------------------------------
		float InputService::ControllerAxis(int id, ControllerButtons::Axes axis) const
		{
			return 0.0f;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::ControllerPressed(int id, KeyCodes::KeyCode button) const
		{
            return ControllerButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::ControllerDown(int id, KeyCodes::KeyCode button) const
		{
            return ControllerButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		bool InputService::ControllerReleased(int id, KeyCodes::KeyCode button) const
		{
            return ControllerButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastControllerPressed(int id) const
		{
            return ControllerButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastControllerDown(int id) const
		{
            return ControllerButtonsEnum::kNone;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode InputService::LastControllerReleased(int id) const
		{
            return ControllerButtonsEnum::kNone;
		}
	}
}
