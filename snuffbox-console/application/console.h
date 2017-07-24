#pragma once

#include "../forms/main_window.h"
#include "../logging/logging.h"

#include <string.h>
#include <wx/font.h>

#include <snuffbox-logging/logging_stream.h>
#include <snuffbox-logging/connection/logging_server.h>

namespace snuffbox
{
	namespace logging
	{
		class LoggingStream;
	}

	namespace console
	{
		class Console;

		/**
		* @class snuffbox::console::ConsoleServer : public snuffbox::LoggingServer
		* @brief The console server that we can log message from the logging stream with
		* @author Daniel Konings
		*/
		class ConsoleServer : public logging::LoggingServer
		{

		public:

			/**
			* @brief Construct by providing the console that we'll append to
			* @param[in] console (snuffbox::console::Console*) The console
			*/
			ConsoleServer(Console* console);

			/**
			* @see snuffbox::logging::LoggingServer::OnConnect
			*/
			void OnConnect(const bool& stream_quit) override;

			/**
			* @see snuffbox::logging::LoggingServer::OnDisconnect
			*/
			void OnDisconnect(const bool& stream_quit) override;

			/**
			* @see snuffbox::logging::LoggingServer::OnLog
			*/
			void OnLog(LogSeverity severity, const char* message, const unsigned char* col_fg, const unsigned char* col_bg) override;

		private:

			Console* console_; //!< The console form running in the wxApp
		};

		/**
		* @class snuffbox::console::Console : public snuffbox::MainWindow
		* @brief The main console window that you can send messages to
		* @author Daniel Konings
		*/
		class Console : public MainWindow
		{

			friend class ConsoleServer;

		protected:

			/**
			* @struct snuffbox::console::Console::LogMessage
			* @brief Used to send messages from the networking thread to the main thread
			* @author Daniel Konings
			*/
			struct LogMessage
			{
				bool bold; //!< Should the message be bolded?
				LogSeverity severity; //!< The severity
				wxString timestamp; //!< The timestamp
				wxString message; //!< The message
				LogColour colour; //!< The colour
				bool repeat; //!< Should the message be repeated?
			};

        public:

			/**
			* @class snuffbox::console::Console::Event : public wxCommandEvent
			* @brief The derived wxCommandEvent to handle all console events
			* @author Daniel Konings
			*/
			class Event : public wxCommandEvent
			{

			public:

				/**
				* @brief Construct through an event type required by a wxCommandEvent
				* @param[in] type (const wxEventType&) The type of command
				* @param[in] id (int) The ID of this command, currently unused
				*/
				Event(const wxEventType& type, int id = 0);

				/**
				* @brief Copy constructor
				*/
				Event(const Event& other);

				/**
				* @brief Clones the data from this event into another
				*/
				Event* Clone() const override;

				/**
				* @brief Sets the message of this event, contains all data to log with
				* @remarks This is the data that gets transferred from thread-to-thread
				* @param[in] message (const snuffbox::console::Console::LogMessage&) The message to transfer
				*/
				void set_message(const LogMessage& message);

				/**
				* @return (const snuffbox::console::Console::LogMessage&) The message of this event
				*/
				const LogMessage& message() const;

			private:

				LogMessage message_; //!< The message of this event
			};

			/**
			* @brief Default constructor, requires a parent window to construct the underlying MainWindow form
			* @param[in] parent (wxWindow*) The parent window to assign to the MainWindow
            * @param[in] port (int) The port to open the connection on
			* @param[in] max_lines (int) The maximum number of lines in the console
			* @remarks After the max lines are reached, the console will clear
			*/
            Console(wxWindow* parent, int port, int max_lines);

			/**
			* @brief Adds a message with a severity and a timestamp to the console
			* @remarks The different severities also change the colour of the log
			* @param[in] severity (snuffbox::console::LogSeverity) The severity to log with
			* @param[in] msg (const wxString&) The message to log
			* @param[in] colour (const snuffbox::console::LogColour&) Optional, this parameter is only used with LogSeverity::kRGB
			*/
			void AddMessage(LogSeverity severity, const wxString& msg, const LogColour& colour = LogColour());

			/**
			* @brief Actually adds the line to the console after thread-to-thread data transfer
			* @param[in] evt (const snuffbox::console::Console::Event&) The event received from wxWidgets
			*/
			void AddLine(const Event& evt);

			/**
            * @brief Called when the user provides input in the input box and sends it
			*/
            void SendInput();

            /**
            * @brief Toggles between Command and JavaScript mode
            */
            void ToggleMode();

			/**
			* @brief Changes the current history index
			* @param[in] dir (int) The direction to move in
			* @return (bool) Is the input box focussed?
			*/
			bool HistoryChange(int dir);

			/**
			* @brief Closes the console window
			*/
			void CloseWindow();

			/**
			* @brief Default destructor, closes the stream
			*/
			~Console();

		protected:

			/**
			* @return (wxString) A generated time stamp of the current time point
			*/
			static wxString CreateTimeStamp();

			const static wxColour BACKGROUND_COLOUR_; //!< The background colour of the console
			const static LogColour LOG_COLOURS_[static_cast<char>(LogSeverity::kCount)]; //!< The list of colours per severity
			const static wxColour TIMESTAMP_COLOUR_; //!< The colour of the timestamp
			const static wxColour SEVERITY_COLOUR_; //!< The colour of the severity
			const static wxColour REPEAT_COLOUR_[2]; //!< The colours for the repeat counter
			const static wxString LOG_PREFIXES_[static_cast<char>(LogSeverity::kCount)]; //!< The list of prefixes per severity
			const static wxString SEVERITY_TO_STRING_[static_cast<char>(LogSeverity::kCount)]; //!< The list of severity names

		private:

			wxFont font_; //!< The font of the console
			unsigned int messages_; //!< The number of messages
			logging::LoggingStream stream_; //!< The logging stream
			ConsoleServer server_; //!< The logging server
			unsigned int max_line_count_; //!< The maximum line count for this console

			wxString last_message_; //!< The last message sent to the console
			LogSeverity last_severity_; //!< The last log severity
			unsigned int repeat_count_; //!< The current repeat count

			/**
			* @struct snuffbox::console::Console::InputHistory
			* @brief A struct to store information about an input provided by the user
			* @author Daniel Konings
			*/
			struct InputHistory
			{
				wxString value; //!< The input value
				int command; //!< The command index used during this input
			};

			wxVector<InputHistory> input_history_; //!< The console input history
			int input_history_index_; //!< The current history index

			bool can_receive_; //!< Can the console receive messages?
			std::mutex receive_mutex_; //!< The mutex for receiving messages
			std::condition_variable receive_cv_; //!< The receive conditional variable
			std::thread send_thread_; //!< The thread to send commands with, without interfering with the log thread
			bool quit_; //!< Has the console quit?
		};

		wxDECLARE_EVENT(CONSOLE_MSG_EVT, Console::Event);
	}
}
