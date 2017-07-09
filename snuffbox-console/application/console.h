#pragma once

#include "../forms/main_window.h"
#include "../logging/logging.h"

#include <string.h>
#include <wx/font.h>

namespace snuffbox
{
	namespace console
	{
		class ConsoleServer;

		/**
		* @class snuffbox::console::Console : public MainWindow
		* @brief The main console window that you can send messages to
		* @author Daniël Konings
		*/
		class Console : public MainWindow
		{

			friend class ConsoleServer;

		protected:

			/**
			* @struct snuffbox::console::Console::LogMessage
			* @brief Used to send messages from the networking thread to the main thread
			* @author Daniël Konings
			*/
			struct LogMessage
			{
				bool bold; //!< Should the message be bolded?
				wxString message; //!< The message
				LogColour colour; //!< The colour
			};

		public:

			/**
			* @class snuffbox::console::Console::Event : public wxCommandEvent
			* @brief The derived wxCommandEvent to handle all console events
			* @author Daniël Konings
			*/
			class Event : public wxCommandEvent
			{

			public:

				/**
				* @brief Construct through an event type required by a wxCommandEvent
				* @param[in] type (const wxEventType&) The type of command
				* @param[in] id (const int&) The ID of this command, currently unused
				*/
				Event(const wxEventType& type, const int& id = 0);

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
			* @param[in] max_lines (const int&) The maximum number of lines in the console, default = 1000
			*/
			Console(wxWindow* parent, const int& max_lines = 1000);

			/**
			* @brief Adds a message with a severity and a timestamp to the console
			* @remarks The different severities also change the colour of the log
			* @param[in] severity (const snuffbox::console::LogSeverity&) The severity to log with
			* @param[in] msg (const std::string&) The message to log
			* @param[in] colour (const snuffbox::console::LogColour&) Optional, this parameter is only used with LogSeverity::kRGB
			*/
			void AddMessage(const LogSeverity& severity, const std::string& msg, const LogColour& colour = LogColour());

			/**
			* @brief Actually adds the line to the console after thread-to-thread data transfer
			* @param[in] evt (const snuffbox::console::Console::Event&) The event received from wxWidgets
			*/
			void AddLine(const Event& evt);

		protected:

			/**
			* @return (std::string) A generated time stamp of the current time point
			*/
			std::string CreateTimeStamp() const;

			const static LogColour LOG_COLOURS_[static_cast<char>(LogSeverity::kCount)]; //!< The list of colours per severity
			const static std::string LOG_PREFIXES_[static_cast<char>(LogSeverity::kCount)]; //!< The list of prefixes per severity

		private:

			wxFont font_; //!< The font of the console
			unsigned int messages_; //!< The number of messages
			unsigned int max_line_count_; //!< The maximum line count for this console
		};

		wxDECLARE_EVENT(CONSOLE_MSG_EVT, Console::Event);
	}
}