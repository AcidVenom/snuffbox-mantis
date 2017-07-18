#pragma once

#include "logging_socket.h"
#include <thread>

namespace snuffbox
{
	namespace logging
	{
		/**
		* @class snuffbox::logging::LoggingClient : public snuffbox::logging::LoggingSocket
		* @brief The client sided socket to setup a logging stream
		* @author Daniël Konings
		*/
		class LoggingClient : public LoggingSocket
		{

		public:

			/**
			* @brief The different command types
			*/
			enum CommandTypes
			{
				kConsole, //!< Console command
				kJavaScript //!< JavaScript command
			};

			/**
			* @brief Default constructor
			*/
			LoggingClient();

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
			* @brief Called when a command is received from the server
			* @param[in] cmd (const snuffbox::logging::LoggingClient::CommandTypes&) The command type
			*/
			virtual void OnCommand(const CommandTypes& cmd, const char* message);

		private:

			std::thread command_thread_;
		};
	}
}