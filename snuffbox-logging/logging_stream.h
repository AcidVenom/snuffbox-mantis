#pragma once

#include <thread>

namespace snuffbox
{
	class LoggingClient;
	class LoggingServer;
	class WinSockWrapper;

	/**
	* @class snuffbox::LoggingStream
	* @brief A stream to send log messages between the engine and the console
	* @author Daniël Konings
	*/
	class LoggingStream
	{

	public:

		/**
		* @brief Default constructor
		* @remarks Initialises WinSock on Windows systems
		*/
		LoggingStream();

		/**
		* @brief Opens a server to stream with a client
		* @param[in] server (snuffbox::LoggingServer*) A logging server socket with its own implementation
		* @param[in] port (const int&) The port to stream on, default = SNUFF_DEFAULT_PORT
		*/
		void Open(LoggingServer* server, const int& port = SNUFF_DEFAULT_PORT);

		/**
		* @brief Opens a stream between a client and an existing server
		* @param[in] client (snuffbox::LoggingClient*) A logging client socket with its own implementation
		* @param[in] port (const int&) The port to stream on, default = SNUFF_DEFAULT_PORT
		* @param[in] ip (const char*) The IP address to stream on, default = "127.0.0.1" (localhost)
		*/
		void Open(LoggingClient* client, const int& port = SNUFF_DEFAULT_PORT, const char* ip = "127.0.0.1");

		/**
		* @brief Closes the stream and kills the connection if it exists
		* @remarks This function call has a slight delay to make sure all log messages were received (SNUFF_SLEEP_SHUTDOWN)
		*/
		void Close();

		/**
		* @brief Sets the error handler function
		* @param[in] void(*func)(const char*) The function that gets a string from the stream
		*/
		void SetErrorHandler(void(*func)(const char*));

	protected:

		/**
		* @brief Called whenever an error occurs, formats the errorcode as a string and passes it to the error handler
		* @param[in] error (const int&) The error code
		*/
		void LogError(const int& error);

	private:

#ifdef SNUFF_WIN32
		WinSockWrapper* win_sock_; //!< The WinSock wrapper
#endif
		bool should_quit_; //!< Should the stream be closed?
		std::thread connection_thread_; //!< The connection thread
		void(*error_handler_)(const char*); //!< The error handler to stream error messages to
	};
}