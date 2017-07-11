#include "cvar_service.h"
#include "log_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		CVarService::CVarService()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::LogAll()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::SetString(const String& name, const String& value)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to set CVar string '{0} -> {1}', but the CVar service is running a null-service", name.c_str(), value.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::SetBoolean(const String& name, const bool& value)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to set CVar boolean '{0} -> {1}', but the CVar service is running a null-service", name.c_str(), value);
		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::SetNumber(const String& name, const float& value)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to set CVar number '{0} -> {1}', but the CVar service is running a null-service", name.c_str(), value);
		}

		//-----------------------------------------------------------------------------------------------
		CVarString* CVarService::GetString(const String& name)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to retrieve CVar string '{0}', but the CVar service is running a null-service", name.c_str());
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		CVarBoolean* CVarService::GetBoolean(const String& name)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to retrieve CVar boolean '{0}', but the CVar service is running a null-service", name.c_str());
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		CVarNumber* CVarService::GetNumber(const String& name)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to retrieve CVar number '{0}', but the CVar service is running a null-service", name.c_str());
			return nullptr;
		}
	}
}