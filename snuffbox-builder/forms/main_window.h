///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/gauge.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/filepicker.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace snuffbox
{
	#define DIR_PICKER_SOURCE_ID 1000
	#define DIR_PICKER_SNUFF_ID 1001
	#define DIR_PICKER_BUILD_ID 1002
	#define BUTTON_START_ID 1003
	#define BUTTON_STOP_ID 1004
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class MainWindow
	///////////////////////////////////////////////////////////////////////////////
	class MainWindow : public wxFrame 
	{
		private:
		
		protected:
			wxPanel* panel_output;
			wxTextCtrl* output_text;
			wxStaticText* label_progress;
			wxGauge* gauge_progress;
			wxStaticText* label_count;
			wxStaticText* label_status;
			wxStaticText* label_current;
			wxPanel* panel_settings;
			wxStaticText* label_source;
			wxTextCtrl* text_source_path;
			wxDirPickerCtrl* dir_picker_source;
			wxStaticText* label_snuff;
			wxDirPickerCtrl* dir_picker_snuff;
			wxStaticText* label_build_dir;
			wxTextCtrl* text_build_path;
			wxDirPickerCtrl* dir_picker_build;
			wxButton* button_start;
			wxButton* button_stop;
			wxStatusBar* status_bar;
		
		public:
			
			MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 602,400 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			
			~MainWindow();
		
	};
	
} // namespace snuffbox

#endif //__MAIN_WINDOW_H__
