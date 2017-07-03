#pragma once

#include "../forms/main_window.h"
#include "../logging/logging.h"

#include <string.h>
#include <wx/font.h>

namespace snuffbox
{
	/**
	* @class snuffbox::Console : public MainWindow
	* @brief The main console window that you can send messages to
	* @author Daniël Konings
	*/
	class Console : public MainWindow
	{

	public:

		/**
		* @struct snuffbox::Console::LogColour
		* @brief Used to create the different logging colours per severity or when user-specified
		* @author Daniël Konings
		*/
		struct LogColour
		{
			wxColour background; //!< The background colour of the text
			wxColour foreground; //!< The foreground colour of the text
		};

		/**
		* @brief Default constructor, requires a parent window to construct the underlying MainWindow form
		* @param[in] parent (wxWindow*) The parent window to assign to the MainWindow
		*/
		Console(wxWindow* parent);

		/**
		* @brief Adds a message with a severity and a timestamp to the console
		* @remarks The different severities also change the colour of the log
		* @param[in] severity (const snuffbox::LogSeverity&) The severity to log with
		* @param[in] msg (const std::string&) The message to log
		* @param[in] colour (const wxColour&) Optional, this parameter is only used with LogSeverity::kRGB
		*/
		void AddMessage(const LogSeverity& severity, const std::string& msg, const LogColour& colour = LogColour());

	protected:

		/**
		* @return (std::string) A generated time stamp of the current time point
		*/
		std::string CreateTimeStamp() const;

		const static LogColour LOG_COLOURS_[static_cast<unsigned char>(LogSeverity::kCount)]; //!< The list of colours per severity
		const static std::string LOG_PREFIXES_[static_cast<unsigned char>(LogSeverity::kCount)]; //!< The list of prefixes per severity

	private:

		wxFont font_; //!< The font of the console
	};
}