#include "console.h"

#include <wx/fontutil.h>
#include <chrono>
#include <ctime>

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	const Console::LogColour Console::LOG_COLOURS_[static_cast<unsigned char>(LogSeverity::kCount)] = {
		{ wxColour{ 255, 255, 255 }, wxColour{ 0, 0, 0 } },
		{ wxColour{ 200, 220, 255 }, wxColour{ 0, 0, 255 } },
		{ wxColour{ 0, 100, 50 }, wxColour{ 0, 255, 100 } },
		{ wxColour{ 125, 100, 0 }, wxColour{ 255, 200, 0 } },
		{ wxColour{ 80, 0, 0 }, wxColour{ 255, 0, 0 } },
		{ wxColour{ 255, 0, 0 }, wxColour{ 255, 255, 255 } }
	};

	//-----------------------------------------------------------------------------------------------
	const std::string Console::LOG_PREFIXES_[static_cast<unsigned char>(LogSeverity::kCount)] = {
		"~",
		"?",
		"+",
		"^",
		"-",
		"!",
		"~"
	};

	//-----------------------------------------------------------------------------------------------
	Console::Console(wxWindow* parent) :
		MainWindow(parent)
	{
		font_ = wxFont(10, wxFontFamily::wxFONTFAMILY_DEFAULT, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
		font_.SetFaceName("Consolas");
	}

	//-----------------------------------------------------------------------------------------------
	void Console::AddMessage(const LogSeverity& severity, const std::string& msg, const LogColour& colour)
	{
		if (severity == LogSeverity::kCount)
		{
			return;
		}

		unsigned char idx = static_cast<unsigned char>(severity);
		const LogColour& col = severity == LogSeverity::kRGB ? colour : LOG_COLOURS_[idx];

		wxRichTextAttr style;
		style.SetFont(font_);
		style.SetFontStyle(wxFontStyle::wxFONTSTYLE_NORMAL);
		style.SetBackgroundColour(col.background);
		style.SetTextColour(col.foreground);
		style.SetFontWeight(severity == LogSeverity::kFatal == true ? wxFontWeight::wxFONTWEIGHT_BOLD : wxFontWeight::wxFONTWEIGHT_NORMAL);
		style.SetParagraphSpacingAfter(0);
		style.SetParagraphSpacingBefore(0);
		style.SetLineSpacing(0);
		
		output_console->BeginStyle(style);
		output_console->WriteText("[" + CreateTimeStamp() + "] " + LOG_PREFIXES_[idx] + " " + msg + '\n');
		output_console->EndStyle();
	}

	//-----------------------------------------------------------------------------------------------
	std::string Console::CreateTimeStamp() const
	{
		std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
		std::time_t now = std::chrono::system_clock::to_time_t(tp);
		tm* time = std::localtime(&now);

		auto FormatTime = [](const int& time)
		{
			std::string formatted = std::to_string(time);
			formatted = formatted.size() == 1 ? "0" + formatted : formatted;

			return formatted;
		};

		std::string min = std::to_string(time->tm_min);

		return FormatTime(time->tm_hour) + ":" + FormatTime(time->tm_min) + ":" + FormatTime(time->tm_sec);
	}
}