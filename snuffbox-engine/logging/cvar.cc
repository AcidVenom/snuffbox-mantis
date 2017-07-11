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
		bool CVar::ParseNumber(char* value, float* buffer)
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
		void CVar::ParseCommandLine(const int& argc, char** argv)
		{
			char* name = nullptr;
			char* val = nullptr;
			float num = 0.0f;

			LogService& log = Services::Get<LogService>();
			String command_line;

			for (int i = 1; i < argc; ++i)
			{
				name = argv[i];
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

			Services::Provide<CVarService>(this);
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