#include "keycodes.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
        void KeyCodes::KeyCodeLoop(KeyCodes::LoopCallback to_call)
		{
            const KeyCodes::KeyCode start = KeyCodesEnum::kAny;
            const KeyCodes::KeyCode max = KeyCodesEnum::kNone;

            snuffbox::engine::KeyCodeLoop<KeyCodes, start, KeyCodes::Key<start>::is_valid, max>::loop(to_call);
		}
	}
}
