#pragma once

#include "logging_socket.h"

namespace snuffbox
{
	class LoggingStream;

	/**
	* @class snuffbox::LoggingServer : public snuffbox::LoggingSocket
	* @brief The server sided socket to setup a logging stream
	* @author Daniël Konings
	*/
	class LoggingServer : public LoggingSocket
	{

		friend class LoggingStream;

	public:

		/**
		* @brief Default constructor
		*/
		LoggingServer();

	protected:

		/**
		* @brief Opens a server socket on a specific port
		* @param[in] port (const int&) The port to start listening at
		*/
		int OpenSocket(const int& port);

		/**
		* @brief Listens for incoming clients
		* @param[in] quit (const bool&) Has the logging stream been closed yet?
		* @return (int) Are we connected yet? Success = 0
		*/
		int Connect(const bool& quit);

		/**
		* @brief Closes the socket so we can re-use it later
		*/
		void CloseSocket();

		/**
		* @brief Checks for incoming packets and sends packets accordingly
		* @return (bool) Are we still connected?
		*/
		bool Update();

		/**
		* @brief Called when the server is connected to a client
		* @param[in] stream_quit (const bool&) Was the stream shutdown yet?
		*/
		virtual void OnConnect(const bool& stream_quit) const;

		/**
		* @brief Called when the server is disconnected from a client or the main stream closes
		* @param[in] stream_quit (const bool&) Was the stream shutdown yet?
		*/
		virtual void OnDisconnect(const bool& stream_quit) const;
	};
}