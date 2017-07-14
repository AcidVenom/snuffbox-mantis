#pragma once

#include <snuffbox-logging/connection/logging_client.h>

#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::LogClient : public snuffbox::logging::LoggingClient
		* @brief The logging client that will handle received commands from the server
		* @author Daniël Konings
		*/
		class LogClient : public logging::LoggingClient
		{

			friend class Log;

		protected:

			/**
			* @see snuffbox::logging::LoggingClient::OnCommand
			*/
			void OnCommand(const CommandTypes& cmd, const char* message) override;

			/**
			* @brief Called when the console sends a command to execute
			* @param[in] message (const char*) The sent command
			*/
			static void OnConsoleCommand(const char* message);

			/**
			* @brief Called when the console sends JavaScript code to execute
			* @param[in] message (const char*) The sent source code
			*/
			static void OnJSCommand(const char* message);

			/**
			* @brief Parses a specified command
			* @param[in] command (const char*) The command to parse
			* @return (bool) Was this a valid command?
			*/
			static bool ParseCommand(const char* command);

			/**
			* @brief Handles a specific command
			* @param[in] command (const String&) The command to handle
			* @param[in] args (const char*) The arguments after the command
			* @return (bool) Was it actually a command, space seperated?
			*/
			static bool HandleCommand(const String& command, const char* args);
		};
	}
}