#include "keyboard.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Keyboard::Keyboard() :
			KeyQueue(KeyCodesEnum::kNone + 1, KeyCodesEnum::kAny)
		{

		}
	}
}
