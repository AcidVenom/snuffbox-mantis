///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 24 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/statline.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace snuffbox
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class MainWindow
	///////////////////////////////////////////////////////////////////////////////
	class MainWindow : public wxFrame 
	{
		private:
		
		protected:
			wxStatusBar* status_bar;
			wxMenuBar* menu_bar;
			wxMenu* file;
			wxMenu* about;
			wxSplitterWindow* splitter_main;
			wxPanel* panel_console;
			wxRichTextCtrl* output_console;
			wxStaticLine* output_size;
			wxPanel* panel_status;
			wxStaticLine* output_size1;
			wxRichTextCtrl* output_status;
			wxTextCtrl* input_box;
			wxChoice* input_type;
			wxButton* input_button;
		
		public:
			
			MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Snuffbox Console"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			
			~MainWindow();
			
			void splitter_mainOnIdle( wxIdleEvent& )
			{
				splitter_main->SetSashPosition( 580 );
				splitter_main->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::splitter_mainOnIdle ), NULL, this );
			}
		
	};
	
} // namespace snuffbox

#endif //__MAIN_WINDOW_H__
