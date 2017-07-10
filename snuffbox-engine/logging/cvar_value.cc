#include "cvar_value.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		CVarString::CVarString(const String& init) :
			CVarValue(init)
		{

		}

		//-----------------------------------------------------------------------------------------------
		CVarBoolean::CVarBoolean(const bool& init) :
			CVarValue(init)
		{

		}

		//-----------------------------------------------------------------------------------------------
		CVarNumber::CVarNumber(const float& init) :
			CVarValue(init)
		{

		}
	}
}