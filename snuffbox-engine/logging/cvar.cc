#include "cvar.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		CVar::CVar()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVar::ParseCommandLine(const int& argc, char** argv)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVar::LogAll()
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetString(const String& name, const String& value)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetBoolean(const String& name, const bool& value)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetNumber(const String& name, const float& value)
		{

		}

		//-----------------------------------------------------------------------------------------------
		CVarString* CVar::GetString(const String& name) const
		{
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		CVarBoolean* CVar::GetBoolean(const String& name) const
		{
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		CVarNumber* CVar::GetNumber(const String& name) const
		{
			return nullptr;
		}
	}
}