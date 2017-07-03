#include "application.h"
#include "console.h"

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	bool ConsoleApp::OnInit()
	{
		console_ = new Console(nullptr);
		console_->Show(true);
		console_->AddMessage(LogSeverity::kInfo, "Looking for an available connection..");
		return true;
	}
}