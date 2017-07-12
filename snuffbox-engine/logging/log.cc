#include "log.h" 
#include "cvar.h"

#ifndef SNUFF_DEBUG
	#ifdef SNUFF_WIN32
		#define SNUFF_ASSERT()	\
		while (true)			\
		{						\
			__debugbreak();		\
		}
	#elif SNUFF_LINUX
		#include <signal.h>
		#define SNUFF_ASSERT()	\
		while (true)			\
		{						\
			raise(SIGTRAP);		\
		}
	#endif
#else
	#define SNUFF_ASSERT() assert(false)
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void Log::Client::OnCommand(const logging::LoggingClient::CommandTypes& cmd, const char* message)
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
		bool Log::Client::ParseCommand(const char* command)
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
		bool Log::Client::HandleCommand(const String& command, const char* args)
		{
			LogService& log = Services::Get<LogService>();
			CVarService& cvar = Services::Get<CVarService>();

			if (strlen(args) > 0 && args[0] != ' ')
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
					if (args[i] == ' ' && split == false)
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
					if (args[i] == ' ')
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

		//-----------------------------------------------------------------------------------------------
		Log::Log() :
			enabled_(true)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Log::Initialise(CVar* cvar)
		{
			CVarBoolean* cvconsole = cvar->Get<CVarBoolean>("console");

			if (cvconsole == nullptr || cvconsole->value() == false)
			{
				enabled_ = false;
				return;
			}

			CVarNumber* cvport = cvar->Get<CVarNumber>("console_port");
			CVarString* cvip = cvar->Get<CVarString>("console_ip");

			int port = cvport != nullptr ? cvport->As<int>() : SNUFF_DEFAULT_PORT;
			const char* ip = cvip != nullptr ? cvip->value().c_str() : "127.0.0.1";

			if (strcmp(ip, "localhost") == 0)
			{
				ip = "127.0.0.1";
			}

			if (cvport == nullptr || cvip == nullptr)
			{
				cvar->Set<CVarNumber>("console_port", static_cast<float>(port));
				cvar->Set<CVarString>("console_ip", ip);
			}

			stream_.Open(&client_, port, ip);
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Shutdown()
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Close();
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Debug(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kDebug, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Info(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kInfo, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Success(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kSuccess, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Warning(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kWarning, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Error(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kError, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Fatal(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kFatal, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::RGB(const String& message, const console::LogColour& colour)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kRGB, message.c_str(), static_cast<int>(message.size()), &colour.background.r, &colour.foreground.r);
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Assert(const bool& expr, const String& message)
		{
			if (expr == false)
			{
				Fatal("[ASSERTION FAILED]\n\t\t\t" + message);
				SNUFF_ASSERT();
			}
		}
	}
}