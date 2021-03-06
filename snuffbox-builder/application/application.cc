#include "application.h"
#include "builder.h"

#include <snuffbox-compilers/utils/glslang_validator.h>

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		bool BuilderApp::OnInit()
		{
			compilers::GLSLangValidator::Initialise();

			builder_ = new Builder(nullptr);
			builder_->Show(true);
			
			return true;
		}

		//-----------------------------------------------------------------------------------------------
		int BuilderApp::OnExit()
		{
			compilers::GLSLangValidator::Shutdown();
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
				int id = evt.GetId();

				if (id != BUTTON_START_ID && id != BUTTON_STOP_ID)
				{
					return -1;
				}

				builder_->OnButton(evt);
				return 1;
			}

			return -1;
		}
	}
}