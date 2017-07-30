#include "buttons.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void MouseButtons::MouseButtonsLoop(KeyCodes::LoopCallback to_call)
		{
			const KeyCodes::KeyCode start = MouseButtons::kLeft;
			const KeyCodes::KeyCode max = MouseButtons::kNone;

			KeyCodeLoop<MouseButtons, start, MouseButtons::Key<start>::is_valid, max>::loop(to_call);
		}

		//-----------------------------------------------------------------------------------------------
		void ControllerButtons::ControllerButtonsLoop(KeyCodes::LoopCallback to_call)
		{
			const KeyCodes::KeyCode start = ControllerButtons::kA;
			const KeyCodes::KeyCode max = ControllerButtons::kNone;

			KeyCodeLoop<ControllerButtons, start, ControllerButtons::Key<start>::is_valid, max>::loop(to_call);
		}
	}
}