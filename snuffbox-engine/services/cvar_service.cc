#include "cvar_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		CVarService::CVarService()
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool CVarService::ParseNumber(char* value, float* buffer)
		{
			static const char* numerical = "0123456789.";
			int dot_count = 0;

			bool is_number = true;

			for (int i = 0; i < strlen(value); ++i)
			{
				is_number = false;
				for (int j = 0; j < strlen(numerical); ++j)
				{
					if (value[i] == numerical[j])
					{
						if (value[i] == '.')
						{
							++dot_count;
						}

						is_number = dot_count <= 1;
						break;
					}
				}

				if (is_number == false)
				{
					return false;
				}
			}

			*buffer = static_cast<float>(std::atof(value));
			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::ParseCommandLine(int argc, char** argv)
		{
			char* name = nullptr;
			char* val = nullptr;
			float num = 0.0f;

			LogService& log = Services::Get<LogService>();
			String command_line;

			for (int i = 1; i < argc; ++i)
			{
				name = argv[i];
				if (name[0] != '-')
				{
					continue;
				}

				name = name + 1;

				command_line += String(name) + " ";

				if (i + 1 < argc)
				{
					val = argv[i + 1];
					command_line += String(val) + " ";

					if (strcmp(val, "true") == 0 || strcmp(val, "false") == 0)
					{
						SetBoolean(name, strcmp(val, "true") == 0);
					}
					else if (ParseNumber(val, &num) == true)
					{
						SetNumber(name, num);
					}
					else
					{
						SetString(name, val);
					}

					++i;
				}
			}
		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::LogAll()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::SetString(const String& name, const String& value)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to set CVar string '{0} -> {1}', but the CVar service is running a null-service", name, value);
		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::SetBoolean(const String& name, bool value)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to set CVar boolean '{0} -> {1}', but the CVar service is running a null-service", name, value);
		}

		//-----------------------------------------------------------------------------------------------
		void CVarService::SetNumber(const String& name, float value)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to set CVar number '{0} -> {1}', but the CVar service is running a null-service", name, value);
		}

		//-----------------------------------------------------------------------------------------------
		CVarString* CVarService::GetString(const String& name)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to retrieve CVar string '{0}', but the CVar service is running a null-service", name);
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		CVarBoolean* CVarService::GetBoolean(const String& name)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to retrieve CVar boolean '{0}', but the CVar service is running a null-service", name);
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		CVarNumber* CVarService::GetNumber(const String& name)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Attempted to retrieve CVar number '{0}', but the CVar service is running a null-service", name);
			return nullptr;
		}
	}
}