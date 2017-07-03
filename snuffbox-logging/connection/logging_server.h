#pragma once

namespace snuffbox
{
	class LoggingStream;

	/**
	* @class snuffbox::LoggingServer
	* @brief The server sided socket to setup a logging stream
	* @author Daniël Konings
	*/
	class LoggingServer
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
		*/
		void Connect(const bool& quit);

		/**
		* @brief Closes the socket so we can re-use it later
		*/
		void CloseSocket();

		/**
		* @brief Called when the server is connected to a client
		*/
		virtual void OnConnect() const;

		/**
		* @brief Called when the server is disconnected from a client or the main stream closes
		*/
		virtual void OnDisconnect() const;

	private:

		int socket_; //!< The socket opened for this server
		int client_; //!< The ID of the client socket if connected to this server
		bool connected_; //!< Has the server socket been connected to the client yet?
	};
}