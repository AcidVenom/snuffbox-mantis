#include "console.h"

#include <snuffbox-logging/logging_stream.h>

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
		const wxColour Console::BACKGROUND_COLOUR_ = wxColour(52, 18, 41);

		//-----------------------------------------------------------------------------------------------
		const LogColour Console::LOG_COLOURS_[static_cast<char>(LogSeverity::kCount)] = {
			{ { BACKGROUND_COLOUR_.Red(), BACKGROUND_COLOUR_.Green(), BACKGROUND_COLOUR_.Blue() },{ 180, 180, 180 } },
			{ { BACKGROUND_COLOUR_.Red(), BACKGROUND_COLOUR_.Green(), BACKGROUND_COLOUR_.Blue() },{ 255, 255, 255 } },
			{ { 0, 100, 50 },{ 0, 255, 100 } },
			{ { 125, 100, 0 },{ 255, 200, 0 } },
			{ { 80, 0, 0 },{ 255, 0, 0 } },
			{ { 255, 0, 0 },{ 255, 255, 255 } }
		};

		//-----------------------------------------------------------------------------------------------
		const wxColour Console::TIMESTAMP_COLOUR_ = wxColour(129, 227, 59);

		//-----------------------------------------------------------------------------------------------
		const wxColour Console::SEVERITY_COLOUR_ = wxColour(117, 156, 206);

		//-----------------------------------------------------------------------------------------------
		const wxColour Console::REPEAT_COLOUR_[2] = {
			wxColour(120, 150, 220, 255),
			wxColour(255, 255, 255, 255)
		};

		//-----------------------------------------------------------------------------------------------
		const wxString Console::LOG_PREFIXES_[static_cast<char>(LogSeverity::kCount)] = {
			"$",
			"?",
			"+",
			"^",
			"-",
			"!",
			"~"
		};

		//-----------------------------------------------------------------------------------------------
		const wxString Console::SEVERITY_TO_STRING_[static_cast<char>(LogSeverity::kCount)] = {
			"Debug",
			"Info",
			"Success",
			"Warning",
			"Error",
			"Fatal",
			"RGB"
		};

		//-----------------------------------------------------------------------------------------------
		Console::Console(wxWindow* parent, logging::LoggingStream* stream, const int& max_lines) :
			MainWindow(parent),
			messages_(0),
			stream_(stream),
			max_line_count_(max_lines),
			last_message_(""),
			last_severity_(LogSeverity::kCount),
			repeat_count_(1)
		{
			font_ = wxFont(10, wxFontFamily::wxFONTFAMILY_DEFAULT, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
#ifdef SNUFF_WIN32
			font_.SetFaceName("Consolas");
#elif SNUFF_LINUX
            font_.SetFaceName("Ubuntu Mono");
#endif
			output_console->SetBackgroundColour(BACKGROUND_COLOUR_);
			input_button->Bind(wxEVT_BUTTON, &Console::OnInput, this);
			
			wxAcceleratorEntry entries[1];
			entries[0].Set(wxACCEL_NORMAL, wxKeyCode::WXK_RETURN, MainWindow::ID_InputButton);

			wxAcceleratorTable accel(1, entries);
			SetAcceleratorTable(accel);

			output_status->AppendTextColumn("Severity");
			output_status->AppendTextColumn("Count");
			
			wxVector<wxVariant> values;
			values.resize(2);

			for (char i = 0; i < static_cast<char>(LogSeverity::kCount); ++i)
			{
				values.at(0) = SEVERITY_TO_STRING_[i];
				values.at(1) = "0";

				output_status->InsertItem(i, values);
			}
			
			Bind(CONSOLE_MSG_EVT, &Console::AddLine, this);
		}

		//-----------------------------------------------------------------------------------------------
		void Console::AddMessage(const LogSeverity& severity, const wxString& msg, const LogColour& colour)
		{
			if (severity == LogSeverity::kCount)
			{
				return;
			}

			bool repeat = strcmp(last_message_.c_str(), msg.c_str()) == 0 ? (last_severity_ != severity ? false : true) : false;
			repeat_count_ = repeat == true ? ++repeat_count_ : 1;

			const LogColour& col = severity == LogSeverity::kRGB ? colour : LOG_COLOURS_[static_cast<char>(severity)];

			LogMessage log;
			log.timestamp = "[" + CreateTimeStamp() + "]";
			log.severity = severity;
			log.message = msg;
			log.bold = severity == LogSeverity::kFatal;
			log.colour = col;
			log.repeat = repeat;

			Event evt(CONSOLE_MSG_EVT, 0);
			evt.set_message(log);

			last_message_ = msg;
			last_severity_ = severity;

			wxPostEvent(this, evt);
		}

		//-----------------------------------------------------------------------------------------------
		void Console::AddLine(const Event& evt)
		{
			const LogMessage& msg = evt.message();

			output_console->BeginSuppressUndo();
			
			unsigned int row = static_cast<unsigned int>(msg.severity);
			wxString value = output_status->GetTextValue(row, 1);
			unsigned int count = atoi(value.c_str()) + 1;
			output_status->SetTextValue(std::to_string(count), row, 1);

			if (msg.repeat == true)
			{
				unsigned int old_repeat = repeat_count_ - 1;
				wxTextPos last = output_console->GetLastPosition();
				wxTextPos to_remove = repeat_count_ > 2 ? std::to_string(old_repeat).size() + 4 : 1;

				output_console->Remove(last - to_remove, last);

				wxRichTextAttr style;
				style.SetFont(font_);
				style.SetFontWeight(wxFontWeight::wxFONTWEIGHT_BOLD);
				style.SetBackgroundColour(REPEAT_COLOUR_[0]);
				style.SetTextColour(REPEAT_COLOUR_[1]);
				style.SetParagraphSpacingAfter(0);
				style.SetParagraphSpacingBefore(0);
				style.SetLineSpacing(0);
				
				output_console->MoveEnd();
				output_console->AppendText(" ");
				output_console->BeginStyle(style);
				output_console->WriteText("(" + std::to_string(repeat_count_) + ")\n");
				output_console->EndStyle();

				output_console->EndSuppressUndo();
				return;
			}

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

			wxColour fg_col, bg_col;

			LogColour::Colour col = msg.colour.foreground;
			fg_col.Set(col.r, col.g, col.b, 255);

			col = msg.colour.background;
			bg_col.Set(col.r, col.g, col.b, 255);

			output_console->MoveEnd();

			wxRichTextAttr style;
			style.SetFont(font_);
			style.SetFontStyle(wxFontStyle::wxFONTSTYLE_NORMAL);
			style.SetParagraphSpacingAfter(0);
			style.SetParagraphSpacingBefore(0);
			style.SetLineSpacing(0);

			style.SetBackgroundColour(BACKGROUND_COLOUR_);
			style.SetTextColour(TIMESTAMP_COLOUR_);

			output_console->BeginStyle(style);
			output_console->WriteText(msg.timestamp + " ");
			output_console->EndStyle();

			style.SetTextColour(SEVERITY_COLOUR_);

			output_console->BeginStyle(style);
			output_console->WriteText(LOG_PREFIXES_[static_cast<char>(msg.severity)] + " ");
			output_console->EndStyle();

			style.SetBackgroundColour(bg_col);
			style.SetTextColour(fg_col);
			style.SetFontWeight(msg.bold == true ? wxFontWeight::wxFONTWEIGHT_BOLD : wxFontWeight::wxFONTWEIGHT_NORMAL);

			output_console->BeginStyle(style);
			output_console->WriteText(msg.message + "\n");
			output_console->EndStyle();

			output_console->ScrollIntoView(output_console->GetLastPosition(), WXK_END);
			++messages_;

			output_console->EndSuppressUndo();
		}

		//-----------------------------------------------------------------------------------------------
		void Console::OnInput(wxCommandEvent& evt)
		{
			if (stream_ != nullptr)
			{
				wxString val = input_box->GetValue();

				if (val.size() == 0)
				{
					return;
				}

				int idx = input_type->GetSelection();
				stream_->SendCommand(
					idx == 0 ? logging::LoggingStream::Commands::kCommand : logging::LoggingStream::Commands::kJavaScript,
					val.c_str(),
					val.size());

				input_box->SetValue("");
			}
		}

		//-----------------------------------------------------------------------------------------------
		wxString Console::CreateTimeStamp()
		{
			std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
			std::time_t now = std::chrono::system_clock::to_time_t(tp);
			tm* time = std::localtime(&now);

			auto FormatTime = [](const int& time)
			{
				wxString formatted = std::to_string(time);
				formatted = formatted.size() == 1 ? "0" + formatted : formatted;

				return formatted;
			};

			return FormatTime(time->tm_hour) + ":" + FormatTime(time->tm_min) + ":" + FormatTime(time->tm_sec);
		}

		//-----------------------------------------------------------------------------------------------
		wxDEFINE_EVENT(CONSOLE_MSG_EVT, Console::Event);
	}
}
