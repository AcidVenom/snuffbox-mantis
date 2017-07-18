#pragma once

#include <time.h>

namespace snuffbox
{
	namespace logging
	{
		class LoggingStream;

		/**
		* @class snuffbox::logging::LoggingSocket
		* @brief The base class for the client- and server sided logging sockets
		* @author Daniël Konings
		*/
		class LoggingSocket
		{

			friend class LoggingStream;

		protected:

			/**
			* @brief The different connection statusses to handle from thread to thread
			*/
			enum ConnectionStatus
			{
				kWaiting, //!< We're waiting for packets and there's a connection
				kDisconnected //!< Either the server or client was disconnected
			};

			/**
			* @brief Default constructor
			*/
			LoggingSocket();

			/**
			* @brief Opens a server socket on a specific port
			* @param[in] port (const int&) The port to start listening at
			*/
			virtual int OpenSocket(const int& port) = 0;

			/**
			* @brief Listens for a connection
			* @param[in] port (const int&) The port to listen at
			* @param[in] ip (const char*) The IP address to listen at, client only
			* @param[in] quit (const bool&) Has the logging stream been closed yet?
			* @return (int) Are we connected yet? Success = 0
			*/
			virtual int Connect(const int& port, const char* ip, const bool& quit) = 0;

			/**
			* @brief Closes the socket
			* @param[in] quit (const bool&) Is the socket being closed because the logging stream has closed?
			*/
			virtual void CloseSocket(const bool& quit) = 0;

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
			* @param[in] buffer (const char*) The buffer to send
			* @param[in] size (const int&) The size of the packet, which will be sent over the connection
			* @param[in] quit (const bool&) Has the logging stream been closed yet?
			* @return (bool) Are we still connected?
			*/
			bool SendPacket(const int& socket, const char* buffer, const int& size, const bool& quit);

			/**
			* @brief Updates the connection between server and client
			* @remarks This should simply receive and send packages
			* @param[in] quit (const bool&) Has the logging stream been closed yet?
			* @return (snuffbox::logging::LoggingSocket::ConnectionStatus) What is the current status of the connection?
			*/
			virtual ConnectionStatus Update(const bool& quit) = 0;

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

		protected:

			const static unsigned int DISCONNECTED_SLEEP_; //!< The sleep when there is no connection to prevent busy waiting

			int socket_; //!< The socket of this client or server
			int other_; //!< The socket ID of the connected client or server
			bool connected_; //!< Is there a connection?

			time_t last_time_; //!< The last time a connection was available
			char buffer_[SNUFF_LOG_BUFFERSIZE + sizeof(char)]; //!< The buffer to receive messages with

			bool skip_; //!< Skip the next wait message as we have a packet instead
		};
	}
}