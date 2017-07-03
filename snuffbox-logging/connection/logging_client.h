#pragma once

namespace snuffbox
{
	class LoggingStream;

	/**
	* @class snuffbox::LoggingClient
	* @brief The client sided socket to setup a logging stream
	* @author Daniël Konings
	*/
	class LoggingClient
	{

		friend class LoggingStream;

	public:

		/**
		* @brief Default constructor
		*/
		LoggingClient();

	protected:

		/**
		* @brief Opens the socket for use
		* @return (int) Was the socket created succesfully? Success = 0
		*/
		int OpenSocket();

		/**
		* @brief Non-blocking connect to be run on a seperate thread
		* @param[in] port (const int&) The port to listen on
		* @param[in] ip (const char*) The IP address to connect with
		* @param[in] quit (const bool&) Has the logging stream been closed yet?
		* @return (int) Are we connected yet? Success = 0
		*/
		int Connect(const int& port, const char* ip, const bool& quit);

		/**
		* @brief Closes the socket so we can re-use it later
		*/
		void CloseSocket();

		/**
		* @brief Called when the client succesfully connects to a server
		* @param[in] stream_quit (const bool&) Was the stream shutdown yet?
		*/
		virtual void OnConnect(const bool& stream_quit) const;

		/**
		* @brief Called when the client disconnects from a server or the stream is closed
		* @param[in] stream_quit (const bool&) Was the stream shutdown yet?
		*/
		virtual void OnDisconnect(const bool& stream_quit) const;

	private:

		int socket_; //!< The socket opened for this client
		int server_; //!< The ID of the server socket if connected to a server
		bool connected_; //!< Has the client socket been connected to the server yet?
	};
}