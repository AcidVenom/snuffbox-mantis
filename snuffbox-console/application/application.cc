#include "application.h"
#include "console.h"

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	Server::Server()
	{

	}

	//-----------------------------------------------------------------------------------------------
	Server::Server(Console* console) :
		console_(console)
	{

	}

	//-----------------------------------------------------------------------------------------------
	void Server::OnConnect() const
	{
		console_->AddMessage(LogSeverity::kSuccess, "SERVER CONNECTED");
	}

	//-----------------------------------------------------------------------------------------------
	void Server::OnDisconnect() const
	{
		console_->AddMessage(LogSeverity::kInfo, "SERVER DISCONNECTED");
	}

	//-----------------------------------------------------------------------------------------------
	bool ConsoleApp::OnInit()
	{
		console_ = new Console(nullptr);
		console_->Show(true);
		console_->AddMessage(LogSeverity::kInfo, "Looking for an available connection..");

		server_ = Server(console_);
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