#include "console.h"

#include <wx/fontutil.h>
#include <chrono>
#include <ctime>

namespace snuffbox
{
	namespace console
	{
		//-----------------------------------------------------------------------------------------------
		Console::Event::Event(const wxEventType& type, const int& id) :
			wxCommandEvent(type, id)
		{

		}

		//-----------------------------------------------------------------------------------------------
		Console::Event::Event(const Event& other) :
			wxCommandEvent(other)
		{
			this->set_message(other.message());
		}

		//-----------------------------------------------------------------------------------------------
		Console::Event* Console::Event::Clone() const
		{
			return new Event(*this);
		}

		//-----------------------------------------------------------------------------------------------
		void Console::Event::set_message(const Console::LogMessage& message)
		{
			message_ = message;
		}

		//-----------------------------------------------------------------------------------------------
		const Console::LogMessage& Console::Event::message() const
		{
			return message_;
		}

		//-----------------------------------------------------------------------------------------------
		const LogColour Console::LOG_COLOURS_[static_cast<char>(LogSeverity::kCount)] = {
			{ { 255, 255, 255 }, { 0, 0, 0 } },
			{ { 200, 220, 255 }, { 0, 0, 255 } },
			{ { 0, 100, 50 }, { 0, 255, 100 } },
			{ { 125, 100, 0 }, { 255, 200, 0 } },
			{ { 80, 0, 0 }, { 255, 0, 0 } },
			{ { 255, 0, 0 }, { 255, 255, 255 } }
		};

		//-----------------------------------------------------------------------------------------------
		const std::string Console::LOG_PREFIXES_[static_cast<char>(LogSeverity::kCount)] = {
			"~",
			"?",
			"+",
			"^",
			"-",
			"!",
			"~"
		};

		//-----------------------------------------------------------------------------------------------
		Console::Console(wxWindow* parent, const int& max_lines) :
			MainWindow(parent),
			messages_(0),
			max_line_count_(max_lines)
		{
			font_ = wxFont(10, wxFontFamily::wxFONTFAMILY_DEFAULT, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
			font_.SetFaceName("Consolas");

			Bind(CONSOLE_MSG_EVT, &Console::AddLine, this);
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

			LogMessage log;
			log.message = "[" + CreateTimeStamp() + "] " + LOG_PREFIXES_[idx] + " " + msg;
			log.bold = severity == LogSeverity::kFatal;
			log.colour = col;

			Event evt(CONSOLE_MSG_EVT, 0);
			evt.set_message(log);

			wxPostEvent(this, evt);
		}

		//-----------------------------------------------------------------------------------------------
		void Console::AddLine(const Event& evt)
		{
			output_console->BeginSuppressUndo();

			if (messages_ + 1 > max_line_count_)
			{
				wxString top = output_console->GetLineText(0);

				output_console->SetEditable(true);
				output_console->Remove(0, top.size());
				output_console->MoveHome();
				output_console->Delete(wxRichTextRange(0, 1));
				output_console->SetEditable(false);

				--messages_;
			}

			const LogMessage& msg = evt.message();
			wxColour fg_col, bg_col;

			LogColour::Colour col = msg.colour.foreground;
			fg_col.Set(col.r, col.g, col.b, 255);

			col = msg.colour.background;
			bg_col.Set(col.r, col.g, col.b, 255);

			wxRichTextAttr style;
			style.SetFont(font_);
			style.SetFontStyle(wxFontStyle::wxFONTSTYLE_NORMAL);
			style.SetBackgroundColour(bg_col);
			style.SetTextColour(fg_col);
			style.SetFontWeight(msg.bold == true ? wxFontWeight::wxFONTWEIGHT_BOLD : wxFontWeight::wxFONTWEIGHT_NORMAL);
			style.SetParagraphSpacingAfter(0);
			style.SetParagraphSpacingBefore(0);
			style.SetLineSpacing(0);

			output_console->MoveEnd();
			output_console->BeginStyle(style);
			output_console->WriteText(msg.message + "\n");
			output_console->EndStyle();

			output_console->ScrollIntoView(output_console->GetLastPosition(), WXK_END);
			++messages_;

			output_console->EndSuppressUndo();
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

		//-----------------------------------------------------------------------------------------------
		wxDEFINE_EVENT(CONSOLE_MSG_EVT, Console::Event);
	}
}