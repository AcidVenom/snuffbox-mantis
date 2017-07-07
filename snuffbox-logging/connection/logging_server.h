#pragma once

#include "logging_socket.h"
#include <logging/logging.h>

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
		* @brief Sends an accept message to the client
		* @param[in] quit (const bool&) Has the logging stream been closed yet?
		* @return (bool) Are we still connected?
		*/
		bool SendAccept(const bool& quit);

		/**
		* @see snuffbox::LoggingSocket::Update
		*/
		ConnectionStatus Update(const bool& quit) override;

		/**
		* @brief Called when a log was received
		* @param[in] severity (const snuffbox::LogSeverity&) The severity being logged with
		* @param[in] message (const char*) The message that was received
		* @param[in] col_fg (const unsigned char*) The colour values for the foreground when logged with an RGB severity, default = nullptr
		* @param[in] col_bg (const unsigned char*) The colour values for the background when logged with an RGB severity, default = nullptr
		*/
		virtual void OnLog(const LogSeverity& severity, const char* message, const unsigned char* col_fg = nullptr, const unsigned char* col_bg = nullptr);

	private:

		LogSeverity severity_; //!< The received severity from the client of the previous packet
	};
}