#include "log_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		int LogService::ConsumeToken(unsigned int& index, const String& str)
		{
			char ch = str.at(index);

			if (ch != '{')
			{
				return -1;
			}

			static String numerical = "01234567890";
			String token_value = "";

			static auto IsNumber = [](const char& c)
			{
				for (unsigned int i = 0; i < numerical.length(); ++i)
				{
					if (c == numerical.at(i))
					{
						return true;
					}
				}

				return false;
			};

			ch = str.at(++index);

			while (IsNumber(ch) == true)
			{
				token_value += ch;
				ch = str.at(++index);
			}

			if (token_value.size() > 0 && ch == '}')
			{
				return std::atoi(token_value.c_str());
			}

			return -1;
		}

		//-----------------------------------------------------------------------------------------------
		LogService::LogService()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Debug(const String& message)
		{
			printf("$ %s\n", message.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Info(const String& message)
		{
			printf("? %s\n", message.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Success(const String& message)
		{
			printf("+ %s\n", message.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Warning(const String& message)
		{
			printf("^ %s\n", message.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Error(const String& message)
		{
			printf("- %s\n", message.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Fatal(const String& message)
		{
			printf("! %s\n", message.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void LogService::RGB(const String& message, const console::LogColour& colour)
		{
			printf("~ %s\n", message.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Assert(const bool& expr, const String& message)
		{
			if (expr == false)
			{
				printf("Assertion failed: %s\n", message.c_str());
				assert(false);
			}
		}
	}
}