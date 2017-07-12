#pragma once

#include "../services/log_service.h"
#include <snuffbox-logging/logging_stream.h>
#include <snuffbox-logging/connection/logging_client.h>

namespace snuffbox
{
	namespace engine
	{
		class SnuffboxApp;
		class Allocator;
		class CVar;

		/**
		* @class snuffbox::engine::Log : public snuffbox::engine::LogService
		* @brief The logging system to log all types of different messages and throw assertions with
		* @author Daniël Konings
		*/
		class Log : public LogService
		{
			/**
			* @class snuffbox::engine::Log::Client : public snuffbox::logging::LoggingClient
			* @brief The logging client that will handle received commands from the server
			* @author Daniël Konings
			*/
			class Client : public logging::LoggingClient
			{

				friend class Log;

			protected:

				/**
				* @see snuffbox::logging::LoggingClient::OnCommand
				*/
				void OnCommand(const CommandTypes& cmd, const char* message) override;

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

			friend class SnuffboxApp;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor
			*/
			Log();

			/**
			* @brief Initialises the log system
			* @param[in] cvar (snuffbox::engine::CVar*) The CVar system
			*/
			void Initialise(CVar* cvar);

			/**
			* @brief Shuts down the logging system
			*/
			void Shutdown();

			/**
			* @see snuffbox::engine::LogService::Debug
			*/
			void Debug(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Info
			*/
			void Info(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Success
			*/
			void Success(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Warning
			*/
			void Warning(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Error
			*/
			void Error(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Fatal
			*/
			void Fatal(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::RGB
			*/
			void RGB(const String& message, const console::LogColour& colour) override;

			/**
			* @see snuffbox::engine::LogService::Assert
			*/
			void Assert(const bool& expr, const String& message) override;

		private:

			bool enabled_; //!< Has the console been enabled?
			Client client_; //!< The logging client
			logging::LoggingStream stream_; //!< The logging stream
		};
	}
}