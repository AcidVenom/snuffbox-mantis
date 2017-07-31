#include "buttons.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void MouseButtons::MouseButtonsLoop(KeyCodes::LoopCallback to_call)
		{
            const KeyCodes::KeyCode start = MouseButtonsEnum::kLeft;
            const KeyCodes::KeyCode max = MouseButtonsEnum::kNone;

            KeyCodeLoop<MouseButtons, start, MouseButtons::Key<start>::is_valid, max>::loop(to_call);
		}

		//-----------------------------------------------------------------------------------------------
		void ControllerButtons::ControllerButtonsLoop(KeyCodes::LoopCallback to_call)
		{
            const KeyCodes::KeyCode start = ControllerButtonsEnum::kA;
            const KeyCodes::KeyCode max = ControllerButtonsEnum::kNone;

            KeyCodeLoop<ControllerButtons, start, ControllerButtons::Key<start>::is_valid, max>::loop(to_call);
		}
	}
}
