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
#include <wx/richtext/richtextctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
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
			wxRichTextCtrl* m_richText4;
			wxTextCtrl* m_textCtrl2;
			wxButton* m_button2;
		
		public:
			
			MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Snuffbox Console"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 612,614 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			
			~MainWindow();
		
	};
	
} // namespace snuffbox

#endif //__MAIN_WINDOW_H__
