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