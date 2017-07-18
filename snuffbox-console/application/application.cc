#include "application.h"
#include "console.h"

namespace snuffbox
{
	namespace console
	{
		//-----------------------------------------------------------------------------------------------
		bool ConsoleApp::OnInit()
		{
			console_ = new Console(nullptr);
			console_->Show(true);
			console_->AddMessage(LogSeverity::kInfo, "Looking for an available connection..");

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		int ConsoleApp::OnExit()
		{
			return 0;
		}

        //-----------------------------------------------------------------------------------------------
        int ConsoleApp::FilterEvent(wxEvent& evt)
        {
            if (evt.GetEventType() == wxEVT_KEY_DOWN)
            {
                int key = static_cast<wxKeyEvent&>(evt).GetKeyCode();

                switch (key)
                {
                case wxKeyCode::WXK_RETURN:
                    console_->SendInput();
                    return 1;

                case wxKeyCode::WXK_TAB:
                    console_->ToggleMode();
                    return 1;

                default:
                    return -1;
                }
            }

            return -1;
        }
	}
}
