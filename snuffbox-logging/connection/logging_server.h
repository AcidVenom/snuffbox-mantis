#pragma once

#include "logging_socket.h"

namespace snuffbox
{
	/**
	* @class snuffbox::LoggingServer : public snuffbox::LoggingSocket
	* @brief The server sided socket to setup a logging stream
	* @author Daniël Konings
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
		* @see snuffbox::LoggingSocket::OpenSocket
		*/
		int OpenSocket(const int& port) override;

		/**
		* @see snuffbox::LoggingSocket::Connect
		*/
		int Connect(const int& port, const char* ip, const bool& quit) override;

		/**
		* @see snuffbox::LoggingSocket::CloseSocket
		*/
		void CloseSocket(const bool& quit) override;

		/**
		* @see snuffbox::LoggingSocket::Update
		*/
		ConnectionStatus Update(const bool& quit) override;
	};
}