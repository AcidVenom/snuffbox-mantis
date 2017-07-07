#pragma once

#include "../defines.h"

#include <snuffbox-logging/logging_stream.h>
#include <snuffbox-logging/connection/logging_server.h>

namespace snuffbox
{
	namespace console
	{
		class Console;

		/**
		* @class snuffbox::console::ConsoleServer : public snuffbox::LoggingServer
		* @brief The console server that we can log message from the logging stream with
		* @author Daniël Konings
		*/
		class ConsoleServer : public logging::LoggingServer
		{

		public:

			/**
			* @brief Default constructor
			*/
			ConsoleServer();

			/**
			* @brief Construct by providing the console that we'll append to
			* @param[in] console (snuffbox::console::Console*) The console
			*/
			ConsoleServer(Console* console);

			/**
			* @see snuffbox::logging::LoggingServer::OnConnect
			*/
			void OnConnect(const bool& stream_quit) const override;

			/**
			* @see snuffbox::logging::LoggingServer::OnDisconnect
			*/
			void OnDisconnect(const bool& stream_quit) const override;

			/**
			* @see snuffbox::logging::LoggingServer::OnLog
			*/
			void OnLog(const LogSeverity& severity, const char* message, const unsigned char* col_fg, const unsigned char* col_bg) override;

		private:

			Console* console_;
		};

		/**
		* @class snuffbox::console::ConsoleApp : public wxApp
		* @brief The wxWidgets application that will contain all the wx windows
		* @author Daniël Konings
		*/
		class ConsoleApp : public wxApp
		{
		public:

			/**
			* @brief Called when the wxApp is initialised
			* @return (bool) Was the initialisation a success?
			*/
			bool OnInit() override;

			/**
			* @brief Called when the wxApp is closed
			* @return (int) The exit code
			*/
			int OnExit() override;

		private:

			Console* console_; //!< The main console window of the application generated through a form
			logging::LoggingStream stream_; //!< The logging stream
			ConsoleServer server_; //!< The logging server
		};
	}
}