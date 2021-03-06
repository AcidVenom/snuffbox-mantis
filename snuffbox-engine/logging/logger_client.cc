#include "logger_client.h"
#include "logger.h" 
#include "cvar.h"

#include <cctype>

#ifdef SNUFF_JAVASCRIPT
#include "../js/js_state_wrapper.h"
#endif

#include "../services/window_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		LoggerClient::LoggerClient(logging::LoggingStream& stream) :
			stream_(stream)
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		void LoggerClient::OnCommand(logging::LoggingClient::CommandTypes cmd, const char* message)
		{
			String msg = message;
			console::LogColour colour;
			colour.foreground = { 200, 0, 200 };
			colour.background = { 0, 0, 0 };

			QueueLog(console::LogSeverity::kRGB, "{0}", msg, colour);

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
		void LoggerClient::FlushLogs()
		{
			while (log_queue_.empty() == false)
			{
				const ToLog& front = log_queue_.front();

				if (front.severity == console::LogSeverity::kRGB)
				{
					stream_.Log(front.severity,
						front.message.c_str(),
						static_cast<int>(front.message.size()),
						reinterpret_cast<const unsigned char*>(&front.colour.background),
						reinterpret_cast<const unsigned char*>(&front.colour.foreground));
				}
				else
				{
					stream_.Log(front.severity,
						front.message.c_str(),
						static_cast<int>(front.message.size()));
				}

				log_queue_.pop();
			}
		}

		//-----------------------------------------------------------------------------------------------
		void LoggerClient::OnConsoleCommand(const char* message)
		{
			if (strcmp("help", message) == 0)
			{
				String commands[] = {
					"set <name> <value> - Sets a CVar by name",
					"get <name> - Outputs a CVar by name",
					"show_all - Outputs all currently registered CVars",
					"clear - Clears the console window",
					"quit - Closes the window and shuts down the application",
					"exit - Closes the console",
					"help - Shows this dialog"
				};

				String help = "\n\nAvailable commands:";
				for (int i = 0; i < sizeof(commands) / sizeof(String); ++i)
				{
					help += "\n\t" + commands[i];
				}

				help += "\n";

				QueueLog(console::LogSeverity::kInfo, help);
				return;
			}
			else if (strcmp(message, "show_all") == 0)
			{
				Services::Get<CVarService>().LogAll();
				return;
			}
			else if (strcmp(message, "quit") == 0)
			{
				Services::Get<WindowService>().Close();
				return;
			}
			else
			{
				if (ParseCommand(message) == true)
				{
					return;
				}
			}

			QueueLog(console::LogSeverity::kError, "Invalid command '{0}', type 'help' for a list of available commands", message);
		}

		//-----------------------------------------------------------------------------------------------
		void LoggerClient::OnJSCommand(const char* message)
		{
#ifndef SNUFF_JAVASCRIPT
			QueueLog(console::LogSeverity::kWarning, "JavaScript is disabled, so the code will not be executed");
#else
            JSStateWrapper* js = JSStateWrapper::Instance();
			engine::String output, error;

            bool success = js->Run(message, "console", &output, &error);

			if (success == true)
			{
				QueueLog(console::LogSeverity::kDebug, output);
				return;
			}

			QueueLog(console::LogSeverity::kError, error);
#endif
		}

		//-----------------------------------------------------------------------------------------------
		bool LoggerClient::ParseCommand(const char* command)
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
		bool LoggerClient::HandleCommand(const String& command, const char* args)
		{
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
					QueueLog(console::LogSeverity::kError, "Invalid syntax for command 'set', usage: set <name> <value>");
					return true;
				}

				if (key.size() > 254 || value.size() > 255)
				{
					QueueLog(console::LogSeverity::kError, "Invalid arguments for command 'set', buffer overflow");
					return true;
				}

				char buffer_key[256];
				memset(buffer_key, '-', sizeof(char));
				memcpy(buffer_key + 1, key.c_str(), key.size());
				memset(buffer_key + 1 + key.size(), '\0', sizeof(char));

				char buffer_value[256];
				memcpy(buffer_value, value.c_str(), value.size());
				memset(buffer_value + value.size(), '\0', sizeof(char));

				char* argv[3] = {
					nullptr,
					buffer_key,
					buffer_value
				};

				cvar.ParseCommandLine(3, argv);

				QueueLog(console::LogSeverity::kDebug, "Set CVar '{0}' to '{1}'", key, value);
			}
			else if (command == "get")
			{
				if (strlen(args) == 0)
				{
					QueueLog(console::LogSeverity::kError, "Invalid syntax for command 'get', usage: get <name>");
					return true;
				}

				for (int i = 0; i < strlen(args); ++i)
				{
					if (std::isspace(args[i]) != 0)
					{
						QueueLog(console::LogSeverity::kError, "Invalid syntax for command 'get', usage: get <name>");
						return true;
					}
				}

				bool found = false;
				String key = args;

				CVarString* str = cvar.Get<CVarString>(key);
				if (str != nullptr)
				{
					QueueLog(console::LogSeverity::kDebug, "String -> {0}", str->value());
					found = true;
				}

				CVarBoolean* boolean = cvar.Get<CVarBoolean>(key);
				if (boolean != nullptr)
				{
					QueueLog(console::LogSeverity::kDebug, "Boolean -> {0}", boolean->value());
					found = true;
				}

				CVarNumber* number = cvar.Get<CVarNumber>(key);
				if (number != nullptr)
				{
					QueueLog(console::LogSeverity::kDebug, "Number -> {0}", number->value());
					found = true;
				}

				if (found == true)
				{
					return true;
				}

				QueueLog(console::LogSeverity::kDebug, "CVar '{0}' is undefined", args);
			}

			return true;
		}
	}
}
