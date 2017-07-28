#include "keycodes.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void KeyCodes::KeyCodeLoop(KeyCodes::LoopCallback to_call)
		{
			snuffbox::engine::KeyCodeLoop<0, KeyCodes::Key<0>::is_valid>::loop(to_call);
		}
	}
}