#pragma once

#include <snuffbox-logging/connection/logging_client.h>

#include "../services/log_service.h"

#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::LoggerClient : public snuffbox::logging::LoggingClient
		* @brief The logging client that will handle received commands from the server
		* @author Daniel Konings
		*/
		class LoggerClient : public logging::LoggingClient
		{

			friend class SnuffboxApp;
			friend class Logger;

		protected:

			/**
			* @brief Initialises the flushing thread
			* @param[in] logger (snuffbox::engine::Logger&) The logging stream
			*/
			LoggerClient(logging::LoggingStream& stream);

			/**
			* @see snuffbox::engine::LoggingSocket::OnConnect
			*/
			virtual void OnConnect(const bool& stream_quit) override;

			/**
			* @see snuffbox::logging::LoggingClient::OnCommand
			*/
			void OnCommand(CommandTypes cmd, const char* message) override;

			/**
			* @brief Basically does a regular log, but queues it up instead
			* @see snuffbox::engine::LogService::FormatString
			*/
			template <typename ... Args>
			void QueueLog(console::LogSeverity severity, const String& message, const Args&... args);

			/**
			* @brief Called when the application is idle
			*/
			void IdleNotification();

			/**
			* @brief Flushes all the logs on a seperate thread
			*/
			void FlushLogs();

			/**
			* @brief Called when the console sends a command to execute
			* @param[in] message (const char*) The sent command
			*/
			void OnConsoleCommand(const char* message);

			/**
			* @brief Called when the console sends JavaScript code to execute
			* @param[in] message (const char*) The sent source code
			*/
			void OnJSCommand(const char* message);

			/**
			* @brief Parses a specified command
			* @param[in] command (const char*) The command to parse
			* @return (bool) Was this a valid command?
			*/
			bool ParseCommand(const char* command);

			/**
			* @brief Handles a specific command
			* @param[in] command (const String&) The command to handle
			* @param[in] args (const char*) The arguments after the command
			* @return (bool) Was it actually a command, space seperated?
			*/
			bool HandleCommand(const String& command, const char* args);

		protected:

			/**
			* @struct snuffbox::engine::LoggerClient::ToLog
			* @brief Used to queue up the logs after a command so they can be executed on a seperate thread
			* @author Daniel Konings
			*/
			struct ToLog
			{
				console::LogSeverity severity; //!< The severity to log with
				String message; //!< The message to log
				console::LogColour colour; //!< If we have an RGB log, store the colour
			};

		public:

			/**
			* @brief Joins the flush thread
			*/
			~LoggerClient();

		private:

			logging::LoggingStream& stream_; //!< The logging TCP stream

			Queue<ToLog> log_queue_; //!< The queue to fill up with logs and to flush in the end

			std::thread flush_thread_; //!< The thread to flush the logs with
			std::mutex flush_mutex_; //!< The mutex for flushing
			std::condition_variable flush_cv_; //!< The condition variable to let the flush thread know there are new logs
		};

		//-----------------------------------------------------------------------------------------------
		template <typename ... Args>
		inline void LoggerClient::QueueLog(console::LogSeverity severity, const String& message, const Args&... args)
		{
			if (connected_ == false)
			{
				return;
			}

			ToLog to_log;
			String formatted = LogService::FormatString(message, &to_log.colour, args...);

			to_log.severity = severity;
			to_log.message = formatted;

			log_queue_.push(to_log);
		}
	}
}
