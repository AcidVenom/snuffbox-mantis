#include "application.h"
#include "builder.h"

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		bool BuilderApp::OnInit()
		{
			builder_ = new Builder(nullptr);
			builder_->Show(true);

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		int BuilderApp::OnExit()
		{
			return 0;
		}

		//-----------------------------------------------------------------------------------------------
		int BuilderApp::FilterEvent(wxEvent& evt)
		{
			wxEventType type = evt.GetEventType();
			if (type == wxEVT_DIRPICKER_CHANGED)
			{
				builder_->OnDirectoryChanged(static_cast<wxFileDirPickerEvent&>(evt));
				return 1;
			}
			else if (type == wxEVT_BUTTON)
			{
				builder_->OnButton(evt);
				return 1;
			}

			return -1;
		}
	}
}