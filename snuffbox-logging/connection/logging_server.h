#pragma once

#include "logging_socket.h"
#include <snuffbox-console/logging/logging.h>

namespace snuffbox
{
	namespace logging
	{
		/**
		* @class snuffbox::logging::LoggingServer : public snuffbox::logging::LoggingSocket
		* @brief The server sided socket to setup a logging stream
		* @author Daniel Konings
		*/
		class LoggingServer : public LoggingSocket
		{

		public:

			/**
			* @brief Default constructor
			*/
			LoggingServer();

		protected:

			/**
			* @see snuffbox::logging::LoggingSocket::OpenSocket
			*/
			int OpenSocket(const int& port) override;

			/**
			* @see snuffbox::logging::LoggingSocket::Connect
			*/
			int Connect(const int& port, const char* ip, const bool& quit) override;

			/**
			* @see snuffbox::logging::LoggingSocket::CloseSocket
			*/
			void CloseSocket(const bool& quit) override;

			/**
			* @see snuffbox::logging::LoggingSocket::Update
			*/
			ConnectionStatus Update(const bool& quit) override;

			/**
			* @brief Called when a log was received
			* @param[in] severity (const snuffbox::console::LogSeverity&) The severity being logged with
			* @param[in] message (const char*) The message that was received
			* @param[in] col_fg (const unsigned char*) The colour values for the foreground when logged with an RGB severity, default = nullptr
			* @param[in] col_bg (const unsigned char*) The colour values for the background when logged with an RGB severity, default = nullptr
			*/
			virtual void OnLog(const console::LogSeverity& severity, const char* message, const unsigned char* col_fg = nullptr, const unsigned char* col_bg = nullptr);
		};
	}
}