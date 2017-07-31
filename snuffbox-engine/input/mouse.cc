#include "mouse.h"
#include "buttons.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Mouse::Mouse() :
			KeyQueue(MouseButtonsEnum::kNone + 1)
		{

		}
	}
}