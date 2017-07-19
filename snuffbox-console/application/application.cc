#include "application.h"
#include "console.h"

namespace snuffbox
{
	namespace console
	{
		//-----------------------------------------------------------------------------------------------
		bool ConsoleApp::OnInit()
		{
            int port = SNUFF_DEFAULT_PORT;

            for (int i = 1; i < argc; ++i)
            {
                if (strcmp(argv[i], "port") == 0)
                {
                    if (i + 1 < argc)
                    {
                        port = atoi(argv[i + 1]);
                        break;
                    }
                }
            }

            console_ = new Console(nullptr, port);

			console_->Show(true);
            if (port != SNUFF_DEFAULT_PORT)
            {
                console_->AddMessage(LogSeverity::kInfo, "Set custom port: " + std::to_string(port));
            }
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

				case wxKeyCode::WXK_UP:
					console_->HistoryChange(-1);
					return -1;

				case wxKeyCode::WXK_DOWN:
					console_->HistoryChange(1);
					return -1;

                default:
                    return -1;
                }
            }

            return -1;
        }
	}
}
