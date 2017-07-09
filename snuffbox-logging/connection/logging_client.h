#pragma once

#include "logging_socket.h"

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
		};
	}
}