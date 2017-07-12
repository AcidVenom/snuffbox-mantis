#include "cvar.h"

#include "../services/log_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		CVar::CVar()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVar::LogAll()
		{
			LogService& log = Services::Get<LogService>();
			log.Log(console::LogSeverity::kInfo, "Console variables:");

			const char* name = nullptr;

			for (int i = 0; i < CVarBase::CVarTypes::kCount; ++i)
			{
				CVarMap& map = cvars_[i];
				for (CVarMap::iterator it = map.begin(); it != map.end(); ++it)
				{
					name = it->first.c_str();

					switch (i)
					{
					case CVarBase::CVarTypes::kString:
						log.Log(console::LogSeverity::kInfo, "\t{0} -> {1}", name, static_cast<CVarString*>(it->second.get())->value().c_str());
						break;

					case CVarBase::CVarTypes::kBoolean:
						log.Log(console::LogSeverity::kInfo, "\t{0} -> {1}", name, static_cast<CVarBoolean*>(it->second.get())->value());
						break;

					case CVarBase::CVarTypes::kNumber:
						log.Log(console::LogSeverity::kInfo, "\t{0} -> {1}", name, static_cast<CVarNumber*>(it->second.get())->value());
						break;
					}
				}
			}
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetString(const String& name, const String& value)
		{
			DoSet<CVarString>(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetBoolean(const String& name, const bool& value)
		{
			DoSet<CVarBoolean>(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetNumber(const String& name, const float& value)
		{
			DoSet<CVarNumber>(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		CVarString* CVar::GetString(const String& name)
		{
			return Find<CVarString>(name);
		}

		//-----------------------------------------------------------------------------------------------
		CVarBoolean* CVar::GetBoolean(const String& name)
		{
			return Find<CVarBoolean>(name);
		}

		//-----------------------------------------------------------------------------------------------
		CVarNumber* CVar::GetNumber(const String& name)
		{
			return Find<CVarNumber>(name);
		}
	}
}