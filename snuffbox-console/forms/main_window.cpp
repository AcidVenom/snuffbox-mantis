///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 28 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "main_window.h"

///////////////////////////////////////////////////////////////////////////
using namespace snuffbox;

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
	
	status_bar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	menu_bar = new wxMenuBar( 0 );
	file = new wxMenu();
	menu_bar->Append( file, wxT("File") ); 
	
	about = new wxMenu();
	menu_bar->Append( about, wxT("About") ); 
	
	this->SetMenuBar( menu_bar );
	
	wxFlexGridSizer* sizer_grid;
	sizer_grid = new wxFlexGridSizer( 2, 1, 0, 0 );
	sizer_grid->AddGrowableCol( 0 );
	sizer_grid->AddGrowableRow( 0 );
	sizer_grid->SetFlexibleDirection( wxBOTH );
	sizer_grid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	splitter_main = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_3DBORDER|wxSP_3DSASH|wxSP_BORDER|wxSP_LIVE_UPDATE );
	splitter_main->Connect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::splitter_mainOnIdle ), NULL, this );
	splitter_main->SetMinimumPaneSize( 200 );
	
	panel_console = new wxPanel( splitter_main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizer_console;
	sizer_console = new wxBoxSizer( wxHORIZONTAL );
	
	output_console = new wxRichTextCtrl( panel_console, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	sizer_console->Add( output_console, 1, wxALL|wxEXPAND, 5 );
	
	output_size = new wxStaticLine( panel_console, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	sizer_console->Add( output_size, 0, wxEXPAND, 5 );
	
	
	panel_console->SetSizer( sizer_console );
	panel_console->Layout();
	sizer_console->Fit( panel_console );
	panel_status = new wxPanel( splitter_main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizer_status;
	sizer_status = new wxBoxSizer( wxHORIZONTAL );
	
	output_size1 = new wxStaticLine( panel_status, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	sizer_status->Add( output_size1, 0, wxEXPAND, 5 );
	
	output_status = new wxRichTextCtrl( panel_status, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	sizer_status->Add( output_status, 1, wxEXPAND | wxALL, 5 );
	
	
	panel_status->SetSizer( sizer_status );
	panel_status->Layout();
	sizer_status->Fit( panel_status );
	splitter_main->SplitVertically( panel_console, panel_status, 580 );
	sizer_grid->Add( splitter_main, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* sizer_input;
	sizer_input = new wxFlexGridSizer( 1, 2, 0, 0 );
	sizer_input->AddGrowableCol( 0 );
	sizer_input->SetFlexibleDirection( wxBOTH );
	sizer_input->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	input_box = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	input_box->SetMaxSize( wxSize( -1,24 ) );
	
	sizer_input->Add( input_box, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* sizer_input_buttons;
	sizer_input_buttons = new wxBoxSizer( wxHORIZONTAL );
	
	sizer_input_buttons->SetMinSize( wxSize( 200,-1 ) ); 
	wxString input_typeChoices[] = { wxT("Console"), wxT("JavaScript") };
	int input_typeNChoices = sizeof( input_typeChoices ) / sizeof( wxString );
	input_type = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, input_typeNChoices, input_typeChoices, 0 );
	input_type->SetSelection( 0 );
	input_type->SetMaxSize( wxSize( -1,24 ) );
	
	sizer_input_buttons->Add( input_type, 0, wxALL, 5 );
	
	input_button = new wxButton( this, wxID_ANY, wxT("Send"), wxDefaultPosition, wxDefaultSize, 0 );
	input_button->SetMaxSize( wxSize( -1,24 ) );
	
	sizer_input_buttons->Add( input_button, 0, wxALL, 5 );
	
	
	sizer_input->Add( sizer_input_buttons, 1, wxALIGN_RIGHT|wxEXPAND|wxRIGHT|wxSHAPED, 5 );
	
	
	sizer_grid->Add( sizer_input, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( sizer_grid );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MainWindow::~MainWindow()
{
}
