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
		CVarBoolean::CVarBoolean(bool init) :
			CVarValue(init)
		{

		}

		//-----------------------------------------------------------------------------------------------
		CVarNumber::CVarNumber(float init) :
			CVarValue(init)
		{

		}
	}
}