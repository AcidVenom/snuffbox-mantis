#include "log_client.h"
#include "log.h" 
#include "cvar.h"

#include <cctype>

#ifdef SNUFF_JAVASCRIPT
#include "../js/js_state_wrapper.h"
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void LogClient::OnCommand(const logging::LoggingClient::CommandTypes& cmd, const char* message)
		{
			String msg = message;
			Services::Get<LogService>().Log(console::LogSeverity::kDebug, "{0}", msg.c_str());

			switch (cmd)
			{
			case CommandTypes::kConsole:
				OnConsoleCommand(msg.c_str());
				break;

			case CommandTypes::kJavaScript:
				OnJSCommand(msg.c_str());
				break;

			default:
				break;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void LogClient::OnConsoleCommand(const char* message)
		{
			LogService& log = Services::Get<LogService>();

			if (strcmp("help", message) == 0)
			{
				String commands[] = {
					"set <name> <value> - Sets a CVar by name",
					"get <name> - Outputs a CVar by name",
					"show_all - Outputs all currently registered CVars",
					"help - Shows this dialog"
				};

				String help = "\n\nAvailable commands:";
				for (int i = 0; i < sizeof(commands) / sizeof(String); ++i)
				{
					help += "\n\t" + commands[i];
				}

				help += "\n";

				log.Log(console::LogSeverity::kInfo, help);
				return;
			}
			else if (strcmp(message, "show_all") == 0)
			{
				Services::Get<CVarService>().LogAll();
				return;
			}
			else
			{
				if (ParseCommand(message) == true)
				{
					return;
				}
			}

			log.Log(console::LogSeverity::kError, "Invalid command '{0}', type 'help' for a list of available commands", message);
		}

		//-----------------------------------------------------------------------------------------------
		void LogClient::OnJSCommand(const char* message)
		{
			LogService& log = Services::Get<LogService>();

#ifndef SNUFF_JAVASCRIPT
			log.Log(console::LogSeverity::kWarning, "JavaScript is disabled, so the code will not be executed");
#else
			JSStateWrapper* js = JSStateWrapper::Instance();
			v8::Isolate* isolate = js->isolate();

			isolate->Enter();

			js->Run(message, "console", true);

			isolate->Exit();
#endif
		}

		//-----------------------------------------------------------------------------------------------
		bool LogClient::ParseCommand(const char* command)
		{
			String commands[] = {
				"set",
				"get"
			};

			int num_commands = sizeof(commands) / sizeof(String);
			char ch = '\0';

			bool valid;
			for (int i = 0; i < num_commands; ++i)
			{
				valid = true;
				for (int j = 0; j < commands[i].size(); ++j)
				{
					ch = commands[i].at(j);
					if (ch != command[j])
					{
						valid = false;
						break;
					}
				}

				if (valid == true)
				{
					return HandleCommand(commands[i], command + commands[i].size());
				}
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool LogClient::HandleCommand(const String& command, const char* args)
		{
			LogService& log = Services::Get<LogService>();
			CVarService& cvar = Services::Get<CVarService>();

			if (strlen(args) == 0 || std::isspace(args[0]) == 0)
			{
				return false;
			}

			++args;

			if (command == "set")
			{
				String key = "";
				String value = "";
				bool split = false;

				for (int i = 0; i < strlen(args); ++i)
				{
					if (std::isspace(args[i]) != 0 && split == false)
					{
						split = true;
						continue;
					}

					if (split == false)
					{
						key += args[i];
						continue;
					}

					value += args[i];
				}

				if (key.size() == 0 || value.size() == 0 || split == false)
				{
					log.Log(console::LogSeverity::kError, "Invalid syntax for command 'set', usage: set <name> <value>");
					return true;
				}

				if (key.size() > 255 || value.size() > 255)
				{
					log.Log(console::LogSeverity::kError, "Invalid arguments for command 'set', buffer overflow");
					return true;
				}

				char buffer_key[256];
				memcpy(buffer_key, key.c_str(), key.size());
				memset(buffer_key + key.size(), '\0', sizeof(char));

				char buffer_value[256];
				memcpy(buffer_value, value.c_str(), value.size());
				memset(buffer_value + value.size(), '\0', sizeof(char));

				char* argv[3] = {
					nullptr,
					buffer_key,
					buffer_value
				};

				cvar.ParseCommandLine(3, argv);

				log.Log(console::LogSeverity::kDebug, "Set CVar '{0}' to '{1}'", key.c_str(), value.c_str());
			}
			else if (command == "get")
			{
				if (strlen(args) == 0)
				{
					log.Log(console::LogSeverity::kError, "Invalid syntax for command 'get', usage: get <name>");
					return true;
				}

				for (int i = 0; i < strlen(args); ++i)
				{
					if (std::isspace(args[i]) != 0)
					{
						log.Log(console::LogSeverity::kError, "Invalid syntax for command 'get', usage: get <name>");
						return true;
					}
				}

				bool found = false;
				String key = args;

				CVarString* str = cvar.Get<CVarString>(key);
				if (str != nullptr)
				{
					log.Log(console::LogSeverity::kDebug, "String -> {0}", str->value().c_str());
					found = true;
				}

				CVarBoolean* boolean = cvar.Get<CVarBoolean>(key);
				if (boolean != nullptr)
				{
					log.Log(console::LogSeverity::kDebug, "Boolean -> {0}", boolean->value());
					found = true;
				}

				CVarNumber* number = cvar.Get<CVarNumber>(key);
				if (number != nullptr)
				{
					log.Log(console::LogSeverity::kDebug, "Number -> {0}", number->value());
					found = true;
				}

				if (found == true)
				{
					return true;
				}

				log.Log(console::LogSeverity::kDebug, "CVar '{0}' is undefined", args);
			}

			return true;
		}
	}
}