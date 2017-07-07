#pragma once

#include <thread>
#include <mutex>
#include <time.h>

#include <logging/logging.h>

namespace snuffbox
{
	class LoggingSocket;
	class LoggingClient;
	class LoggingServer;

#ifdef SNUFF_WIN32
	class WinSockWrapper;
#endif

	/**
	* @class snuffbox::LoggingStream
	* @brief A stream to send log messages between the engine and the console
	* @author Daniël Konings
	*/
	class LoggingStream
	{

	public:

		/**
		* @brief The different server/client commands
		*/
		enum Commands : char
		{
			kWaiting, //!< When both client and server are idle
			kAccept, //!< When the server accepts a message
			kLog, //!< When the client wants to log something to the server
			kCommand, //!< When the server wants to execute a command on the client
			kJavaScript, //!< When the server wants to execute JavaScript on the client
			kProcessing //!< When either the client or server is processing
		};

		/**
		* @struct snuffbox::LoggingStream::PacketHeader
		* @brief A structure to send information of what to receive or send between a connected client and server
		* @author Daniël Konings
		*/
		struct PacketHeader
		{
			Commands command; //!< The command to execute
			LogSeverity severity; //!< The severity to log with, if applicable
			int size; //!< The size of the upcoming buffer
		};

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
		* @brief Starts streaming between a client and a server
		* @remarks This will start a connection thread next to the main thread
		* @param[in] socket (snuffbox::LoggingSocket*) The client or server socket to stream with
		* @param[in] port (const int&) The port to stream on
		* @param[in] ip (const char*) The IP address to stream to, client only
		* @param[in] is_server (const bool&) Is the socket a server?
		*/
		void Start(LoggingSocket* socket, const int& port, const char* ip);

		/**
		* @brief Starts the connection thread
		* @see snuffbox::LoggingStream::Start
		*/
		void RunThread(LoggingSocket* socket, const int& port, const char* ip);

		/**
		* @brief Logs a message, client only
		* @param[in] severity (const snuffbox::LogSeverity&) The severity to log with
		* @param[in] message (const char*) The message to send
		* @param[in] size (const int&) The message size
		* @param[in] col_fg (const unsigned char*) The foreground colour to log with, default = nullptr
		* @param[in] col_bg (const unsigned char*) The background colour to log with, default = nullptr
		* @remarks Colour parameters are only applicable for the RGB severity
		*/
		void Log(const LogSeverity& severity, const char* message, const int& size, const unsigned char* col_fg = nullptr, const unsigned char* col_bg = nullptr);

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
		void LogError(const int& error) const;

	private:

#ifdef SNUFF_WIN32
		WinSockWrapper* win_sock_; //!< The WinSock wrapper
#endif
		bool should_quit_; //!< Should the stream be closed?
		bool is_server_; //!< Is this stream server-sided?
		LoggingSocket* socket_; //!< The socket that started this stream
		std::thread connection_thread_; //!< The connection thread
		std::mutex connection_mutex_; //!< The connection mutex
		void(*error_handler_)(const char*); //!< The error handler to stream error messages to
	};
}