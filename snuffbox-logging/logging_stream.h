#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <time.h>

#include <snuffbox-console/logging/logging.h>

namespace snuffbox
{
	namespace logging
	{
		class LoggingSocket;
		class LoggingClient;
		class LoggingServer;

#ifdef SNUFF_WIN32
		class WinSockWrapper;
#endif

		/**
		* @class snuffbox::logging::LoggingStream
		* @brief A stream to send log messages between the engine and the console
		* @author Daniel Konings
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
				kLog, //!< When the client wants to log something to the server
				kCommand, //!< When the server wants to execute a command on the client
				kJavaScript, //!< When the server wants to execute JavaScript on the client
				kCount //!< The number of commands
			};

			/**
			* @struct snuffbox::logging::Packet
			* @brief A packet structure to send over the logging connection
			* @author Daniel Konings
			*/
			struct Packet
			{
				Commands command;
				char buffer[SNUFF_LOG_BUFFERSIZE - sizeof(Commands)];
			};

			/**
			* @brief Default constructor
			* @remarks Initialises WinSock on Windows systems
			*/
			LoggingStream();

			/**
			* @brief Opens a server to stream with a client
			* @param[in] server (snuffbox::logging::LoggingServer*) A logging server socket with its own implementation
			* @param[in] port (int) The port to stream on
			*/
			void Open(LoggingServer* server, int port);

			/**
			* @brief Opens a stream between a client and an existing server
			* @param[in] client (snuffbox::logging::LoggingClient*) A logging client socket with its own implementation
			* @param[in] port (int) The port to stream on
			* @param[in] ip (const char*) The IP address to stream on
			*/
			void Open(LoggingClient* client, int port, const char* ip);

			/**
			* @brief Starts streaming between a client and a server
			* @remarks This will start a connection thread next to the main thread
			* @param[in] socket (snuffbox::logging::LoggingSocket*) The client or server socket to stream with
			* @param[in] port (int) The port to stream on
			* @param[in] ip (const char*) The IP address to stream to, client only
			* @param[in] is_server (bool) Is the socket a server?
			*/
			void Start(LoggingSocket* socket, int port, const char* ip);

			/**
			* @brief Starts the connection thread
			* @see snuffbox::logging::LoggingStream::Start
			*/
			void RunThread(LoggingSocket* socket, int port, const char* ip);

			/**
			* @brief Logs a message, client only
			* @param[in] severity (snuffbox::console::LogSeverity) The severity to log with
			* @param[in] message (const char*) The message to send
			* @param[in] size (int) The message size
			* @param[in] col_bg (const unsigned char*) The background colour to log with, default = nullptr
			* @param[in] col_fg (const unsigned char*) The foreground colour to log with, default = nullptr
			* @remarks Colour parameters are only applicable for the RGB severity
			*/
			void Log(console::LogSeverity severity, const char* message, int size, const unsigned char* col_bg = nullptr, const unsigned char* col_fg = nullptr);

			/**
			* @brief Sends a command to the client, server only
			* @param[in] cmd (const snuffbox::logging::LoggingStream::Commands&) The command type
			* @param[in] message (const char*) The message to send
			* @param[in] size (int) The size of the message
			*/
			void SendCommand(const Commands& cmd, const char* message, int size);

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

			/**
			* @return (bool) Is the stream connected?
			*/
			bool Connected() const;

		protected:

			/**
			* @brief Called whenever an error occurs, formats the errorcode as a string and passes it to the error handler
			* @param[in] error (int) The error code
			*/
			void LogError(int error) const;

			/**
			* @brief Sends a packet from this stream's socket to a specified socket
			* @param[in] cmd (snuffbox::logging::LoggingStream::Commands) The command to send
			* @param[in] other (int) The other socket to send to
			* @param[in] buffer (const char*) The buffer to send
			* @param[in] size (int) The size of the buffer
			*/
			void Send(Commands cmd, int other, const char* buffer, int size);

			static const unsigned int STARTUP_SLEEP_; //!< The client-sided wait time so we can receive initialisation logs
			static const unsigned int WAIT_SLEEP_; //!< The sleep to prevent busy-waiting while the console isn't doing anything
			static const unsigned int SHUTDOWN_SLEEP_; //!< The sleep before the stream shuts down so we can still receive some last logs

		private:

#ifdef SNUFF_WIN32
			WinSockWrapper* win_sock_; //!< The WinSock wrapper
#endif
			bool should_quit_; //!< Should the stream be closed?
			bool is_server_; //!< Is this stream server-sided?
			LoggingSocket* socket_; //!< The socket that started this stream
			std::thread connection_thread_; //!< The connection thread
			std::condition_variable_any connection_cv_; //!< The conditional variable for messaging when a socket is ready to send
			std::mutex connection_mutex_; //!< The connection mutex
			void(*error_handler_)(const char*); //!< The error handler to stream error messages to
		};
	}
}