#pragma once

#include "service.h"
#include "services.h"
#include "../core/eastl.h"

#include <snuffbox-console/logging/logging.h>
#include <sstream>

#ifdef SNUFF_WIN32
#include <Windows.h>
#undef RGB
#endif

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::LogService : public snuffbox::engine::Service<snuffbox::engine::ServiceIDs::kLogService>
		* @brief The logging service interface for all log services to use
		* @author Daniel Konings
		*/
		class LogService : public Service<ServiceIDs::kLogService>
		{
			
			friend class Services;

		protected:

			/**
			* @brief Default constructor, creates a null service
			*/
			LogService();

			/**
			* @brief Consumes a token of format '{number}' from a specified string and increments the current index
			* @param[in] index (unsigned int&) The current index, will be changed after consumption
			* @param[in] str (const snuffbox::engine::String&) The string to consume from
			* @return (int) The number of the token consumed, or if none were found -1
			*/
			static int ConsumeToken(unsigned int& index, const String& str);

			/**
			* @brief Converts different values to a string
			* @param[in] value (const T&) The value to convert
			*/
			template <typename T>
			static String ToString(const T& value);

			/**
			* @brief The end of the argument recurssion
			* @param[in] parsed (snuffbox::engine::Vector<snuffbox::engine::String>&) The parsed values list
			* @param[out] colour (snuffbox::console::LogColour&) The colour for RGB logging stored in the arguments
			* @return (unsigned int) Returns 0, since no arguments are evaluated
			*/
			static unsigned int GetArgument(Vector<String>& parsed, console::LogColour* colour);

			/**
			* @brief Retrieves an argument from the provided formatting arguments if there is only one argument left
			* @param[in] parsed (snuffbox::engine::Vector<snuffbox::engine::String>&) The parsed values list
			* @param[out] colour (snuffbox::console::LogColour&) The colour for RGB logging stored in the arguments
			* @param[in] last (const T&) The current argument being evaluated
			* @return (unsigned int) The number of arguments evaluated
			*/
			template <typename T>
			static unsigned int GetArgument(Vector<String>& parsed, console::LogColour* colour, const T& last);

			/**
			* @brief Retrieves an argument from the provided formatting arguments if there are still arguments left
			* @param[in] parsed (snuffbox::engine::Vector<snuffbox::engine::String>&) The parsed values list
			* @param[out] colour (snuffbox::console::LogColour&) The colour for RGB logging stored in the arguments
			* @param[in] first (const T&) The current argument being evaluated
			* @param[in] others (const Args&...) The other arguments
			* @return (unsigned int) The number of arguments evaluated
			*/
			template <typename T, typename ... Args>
			static unsigned int GetArgument(Vector<String>& parsed, console::LogColour* colour, const T& first, const Args&... others);

			/**
			* @brief Formats a provided string
			* @param[in] str (const snuffbox::engine::String&) The string to format
			* @param[out] colour (snuffbox::console::LogColour&) The colour for RGB logging stored in the arguments
			* @param[in] args (const Args&...) The formatting arguments
			* @return (snuffbox::engine::String) The formatted result
			*/
			template <typename ... Args>
			static String FormatString(const String& str, console::LogColour* colour, const Args&... args);

		public:

			/**
			* @brief Delete copy constructor
			*/
			LogService(const LogService& other) = delete;

			/**
			* @brief Delete assignment operator
			*/
			LogService operator=(const LogService& other) = delete;

			/**
			* @brief Logs with a specified severity and message
			* @param[in] severity (const snuffbox::console::LogSeverity&) The severity to log with
			* @param[in] message (const snuffbox::engine::String&) The message to log
			* @param[in] args (const Args&...) The arguments for formatting
			* @remarks The formatting should be a token in the format of '{number}', e.g. 'This is a test {0}, this is as well {1}'
			*/
			template <typename ... Args>
			void Log(const console::LogSeverity& severity, const String& message, const Args&... args);

			/**
			* @brief Asserts an expression and logs a fatal error if the assertion fails
			* @param[in] expr (const bool&) The expression to evaluate
			* @param[in] message (const snuffbox::engine::String&) The message to log if the assertion fails
			* @param[in] args (const Args&...) The arguments for formatting
			* @remarks The formatting should be a token in the format of '{number}', e.g. 'This is a test {0}, this is as well {1}'
			*/
			template <typename ... Args>
			void Assert(const bool& expr, const String& message, const Args&... args);

		protected:

			/**
			* @brief Prints a debug message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Debug(const String& message);

			/**
			* @brief Prints an info message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Info(const String& message);

			/**
			* @brief Prints a success message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Success(const String& message);

			/**
			* @brief Prints a warning message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Warning(const String& message);

			/**
			* @brief Prints an error message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Error(const String& message);

			/**
			* @brief Prints a fatal message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Fatal(const String& message);

			/**
			* @brief Prints an RGB message with a specified colour
			* @param[in] message (const snuffbox::engine::String&) The message to log
			* @param[in] colour (const snuffbox::console::LogColour&) The colour to log with
			*/
			virtual void RGB(const String& message, const console::LogColour& colour);

			/**
			* @brief Cross-platform assert with a message
			* @param[in] expr (const bool&) The expression to evaluate
			* @param[in] message (const snuffbox::engine::String&) The message to log with fatal severity if the evaluation was false
			*/
			virtual void DoAssert(const bool& expr, const String& message);
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline String LogService::ToString(const T& value)
		{
			std::ostringstream stream;
			stream << value;

			return stream.str().c_str();
		}

		//-----------------------------------------------------------------------------------------------
		template<>
		inline String LogService::ToString<bool>(const bool& value)
		{
			return value == true ? "true" : "false";
		}

		//-----------------------------------------------------------------------------------------------
		template<>
		inline String LogService::ToString<String>(const String& value)
		{
			return ToString<const char*>(value.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		inline unsigned int LogService::GetArgument(Vector<String>& parsed, console::LogColour* colour)
		{
			return 0;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline unsigned int LogService::GetArgument(Vector<String>& parsed, console::LogColour* colour, const T& last)
		{
			parsed.push_back(ToString(last));
			return 1;
		}

		//-----------------------------------------------------------------------------------------------
		template <>
		inline unsigned int LogService::GetArgument<console::LogColour>(Vector<String>& parsed, console::LogColour* colour, const console::LogColour& last)
		{
			if (colour == nullptr)
			{
				return 0;
			}

			*colour = last;
			return 0;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T, typename ... Args>
		inline unsigned int LogService::GetArgument(Vector<String>& parsed, console::LogColour* colour, const T& first, const Args&... others)
		{
			parsed.push_back(ToString(first));
			return GetArgument(parsed, colour, others...) + 1;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename ... Args>
		inline String LogService::FormatString(const String& str, console::LogColour* colour, const Args&... args)
		{
			Vector<String> parsed;
			unsigned int nargs = GetArgument(parsed, colour, args...);

			String value = "";

			unsigned int length = static_cast<unsigned int>(str.length());
			unsigned int i = 0;

			int token = -1;

			while (i != length)
			{
				token = ConsumeToken(i, str);

				if (token != -1)
				{
					if (token < static_cast<int>(nargs))
					{
						value += parsed.at(token);
					}
				}
				else
				{
					value += str.at(i);
				}

				++i;
			}

			return value;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename ... Args>
		inline void LogService::Log(const console::LogSeverity& severity, const String& message, const Args&... args)
		{
			assert(severity < console::LogSeverity::kCount);

			console::LogColour colour;
			String formatted = FormatString(message, &colour, args...);

#if defined SNUFF_WIN32 && defined SNUFF_DEBUG
			OutputDebugStringA((formatted + "\n").c_str());
#endif

#ifdef SNUFF_DEBUG
			printf("%s\n", formatted.c_str());
#endif

			switch (severity)
			{
			case console::LogSeverity::kDebug:
				Debug(formatted);
				break;

			case console::LogSeverity::kInfo:
				Info(formatted);
				break;

			case console::LogSeverity::kSuccess:
				Success(formatted);
				break;

			case console::LogSeverity::kWarning:
				Warning(formatted);
				break;

			case console::LogSeverity::kError:
				Error(formatted);
				break;

			case console::LogSeverity::kFatal:
				Fatal(formatted);
				break;

			case console::LogSeverity::kRGB:
				RGB(formatted, colour);
				break;
			}
		}

		//-----------------------------------------------------------------------------------------------
		template <typename ... Args>
		inline void LogService::Assert(const bool& expr, const String& message, const Args&... args)
		{
			String formatted = FormatString(message, nullptr, args...);

#ifdef SNUFF_DEBUG
			if (expr == false)
			{
				printf("Assertion failed: %s\n", formatted.c_str());
			}
#endif

			DoAssert(expr, formatted);
		}
	}
}