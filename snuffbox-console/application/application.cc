#include "application.h"
#include "console.h"

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	ConsoleServer::ConsoleServer()
	{

	}

	//-----------------------------------------------------------------------------------------------
	ConsoleServer::ConsoleServer(Console* console) :
		console_(console)
	{

	}

	//-----------------------------------------------------------------------------------------------
	void ConsoleServer::OnConnect(const bool& stream_quit) const
	{
		if (stream_quit == false)
		{
			console_->AddMessage(LogSeverity::kSuccess, "Succesfully connected to the engine");
		}
	}

	//-----------------------------------------------------------------------------------------------
	void ConsoleServer::OnDisconnect(const bool& stream_quit) const
	{
		if (stream_quit == false)
		{
			console_->AddMessage(LogSeverity::kInfo, "Disconnected from the engine, looking for a new connection..");
		}
	}

	//-----------------------------------------------------------------------------------------------
	void ConsoleServer::OnLog(const LogSeverity& severity, const char* message, const unsigned char* col_fg, const unsigned char* col_bg)
	{
		if (severity == LogSeverity::kRGB && col_fg != nullptr && col_bg != nullptr)
		{
			Console::LogColour col;
			col.foreground.Set(col_fg[0], col_fg[1], col_fg[2], 255);
			col.background.Set(col_bg[0], col_bg[1], col_bg[2], 255);

			console_->AddMessage(severity, message, col);
			return;
		}

		console_->AddMessage(severity, message);
	}

	//-----------------------------------------------------------------------------------------------
	bool ConsoleApp::OnInit()
	{
		console_ = new Console(nullptr);
		console_->Show(true);
		console_->AddMessage(LogSeverity::kInfo, "Looking for an available connection..");

		server_ = ConsoleServer(console_);
		stream_.Open(&server_);

		return true;
	}

	//-----------------------------------------------------------------------------------------------
	int ConsoleApp::OnExit()
	{
		stream_.Close();
		return 0;
	}
}