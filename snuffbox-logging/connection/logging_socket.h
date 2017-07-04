#pragma once

#include <time.h>

namespace snuffbox
{
	class LoggingSocket
	{

	protected:

		/**
		* @brief Default constructor
		*/
		LoggingSocket();

		/**
		* @brief Has the connection been timed out?
		* @param[in] timeout (const unsigned int&) The timeout in seconds
		* @return (bool) Has the connection between a client and server timed out?
		*/
		bool TimedOut(const unsigned int& timeout = SNUFF_LOG_TIMEOUT) const;

		/**
		* @brief Receive a packet from a specified socket
		* @remarks When the connection is not established this method will return false
		* @param[in] socket (const int&) The socket to receive packets from
		* @param[in] expected_size (const int&) The expected size of the packet, which will be written to the buffer
		* @param[in] quit (const bool&) Has the logging stream been closed yet?
		* @return (bool) Are we still connected?
		*/
		bool ReceivePacket(const int& socket, const int& expected_size, const bool& quit);

		/**
		* @brief Sends a packet to a specified socket
		* @remarks When the connection is not established this method will return false
		* @param[in] socket (const int&) The socket to send packets to
		* @param[in] size (const int&) The size of the packet, which will be sent over the connection
		* @param[in] quit (const bool&) Has the logging stream been closed yet?
		* @return (bool) Are we still connected?
		*/
		bool SendPacket(const int& socket, const int& size, const bool& quit);

	protected:

		int socket_; //!< The socket of this client or server
		int other_; //!< The socket ID of the connected client or server
		bool connected_; //!< Is there a connection?

		time_t last_time_; //!< The last time a connection was available
		char buffer_[SNUFF_LOG_BUFFERSIZE]; //!< The buffer to receive messages with
	};
}