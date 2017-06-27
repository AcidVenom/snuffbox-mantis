#include "application.h"
#include "../forms/main_window.h"

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	bool ConsoleApp::OnInit()
	{
		main_window_ = new MainWindow(nullptr);
		main_window_->Show(true);
		return true;
	}
}