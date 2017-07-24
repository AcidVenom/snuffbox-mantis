///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "main_window.h"

///////////////////////////////////////////////////////////////////////////
using namespace snuffbox;

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );
	
	wxFlexGridSizer* sizer_main;
	sizer_main = new wxFlexGridSizer( 2, 1, 0, 0 );
	sizer_main->AddGrowableCol( 0 );
	sizer_main->AddGrowableRow( 0 );
	sizer_main->SetFlexibleDirection( wxBOTH );
	sizer_main->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	panel_output = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* sizer_output;
	sizer_output = new wxFlexGridSizer( 2, 1, 0, 0 );
	sizer_output->AddGrowableCol( 0 );
	sizer_output->AddGrowableRow( 0 );
	sizer_output->SetFlexibleDirection( wxBOTH );
	sizer_output->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	output_text = new wxTextCtrl( panel_output, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	output_text->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	output_text->SetBackgroundColour( wxColour( 52, 18, 41 ) );
	
	sizer_output->Add( output_text, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* sizer_progress;
	sizer_progress = new wxBoxSizer( wxHORIZONTAL );
	
	label_progress = new wxStaticText( panel_output, wxID_ANY, wxT("Progress"), wxDefaultPosition, wxDefaultSize, 0 );
	label_progress->Wrap( -1 );
	sizer_progress->Add( label_progress, 0, wxALL, 5 );
	
	gauge_progress = new wxGauge( panel_output, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	gauge_progress->SetValue( 0 ); 
	sizer_progress->Add( gauge_progress, 0, wxALL, 5 );
	
	label_count = new wxStaticText( panel_output, wxID_ANY, wxT("0/0"), wxDefaultPosition, wxDefaultSize, 0 );
	label_count->Wrap( -1 );
	sizer_progress->Add( label_count, 0, wxALL, 5 );
	
	
	sizer_progress->Add( 0, 0, 1, wxEXPAND, 5 );
	
	label_status = new wxStaticText( panel_output, wxID_ANY, wxT("Status:"), wxDefaultPosition, wxDefaultSize, 0 );
	label_status->Wrap( -1 );
	sizer_progress->Add( label_status, 0, wxALL, 5 );
	
	label_current = new wxStaticText( panel_output, wxID_ANY, wxT("Stopped"), wxDefaultPosition, wxDefaultSize, 0 );
	label_current->Wrap( -1 );
	sizer_progress->Add( label_current, 0, wxALL, 5 );
	
	
	sizer_output->Add( sizer_progress, 1, wxEXPAND, 5 );
	
	
	panel_output->SetSizer( sizer_output );
	panel_output->Layout();
	sizer_output->Fit( panel_output );
	sizer_main->Add( panel_output, 1, wxEXPAND | wxALL, 5 );
	
	panel_settings = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* sizer_settings;
	sizer_settings = new wxFlexGridSizer( 0, 4, 0, 0 );
	sizer_settings->AddGrowableCol( 0 );
	sizer_settings->AddGrowableCol( 1 );
	sizer_settings->SetFlexibleDirection( wxBOTH );
	sizer_settings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* sizer_source;
	sizer_source = new wxBoxSizer( wxVERTICAL );
	
	label_source = new wxStaticText( panel_settings, wxID_ANY, wxT("Source directory"), wxDefaultPosition, wxDefaultSize, 0 );
	label_source->Wrap( -1 );
	sizer_source->Add( label_source, 0, wxALL, 5 );
	
	wxFlexGridSizer* sizer_source_picker;
	sizer_source_picker = new wxFlexGridSizer( 2, 2, 0, 0 );
	sizer_source_picker->AddGrowableCol( 0 );
	sizer_source_picker->SetFlexibleDirection( wxBOTH );
	sizer_source_picker->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	text_source_path = new wxTextCtrl( panel_settings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sizer_source_picker->Add( text_source_path, 0, wxALL|wxEXPAND, 5 );
	
	dir_picker_source = new wxDirPickerCtrl( panel_settings, DIR_PICKER_SOURCE_ID, wxEmptyString, wxT("Choose source directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST );
	sizer_source_picker->Add( dir_picker_source, 0, wxALL|wxEXPAND, 5 );
	
	label_snuff = new wxStaticText( panel_settings, wxID_ANY, wxT("Create '.snuff' in folder"), wxDefaultPosition, wxDefaultSize, 0 );
	label_snuff->Wrap( -1 );
	sizer_source_picker->Add( label_snuff, 0, wxALL, 5 );
	
	dir_picker_snuff = new wxDirPickerCtrl( panel_settings, DIR_PICKER_SNUFF_ID, wxEmptyString, wxT("Promote a folder to a source directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST|wxDIRP_SMALL );
	sizer_source_picker->Add( dir_picker_snuff, 0, wxALL, 5 );
	
	
	sizer_source->Add( sizer_source_picker, 1, wxEXPAND, 5 );
	
	
	sizer_settings->Add( sizer_source, 1, wxEXPAND, 5 );
	
	wxBoxSizer* sizer_build;
	sizer_build = new wxBoxSizer( wxVERTICAL );
	
	label_build_dir = new wxStaticText( panel_settings, wxID_ANY, wxT("Build directory"), wxDefaultPosition, wxDefaultSize, 0 );
	label_build_dir->Wrap( -1 );
	sizer_build->Add( label_build_dir, 0, wxALL, 5 );
	
	wxFlexGridSizer* sizer_build_picker;
	sizer_build_picker = new wxFlexGridSizer( 1, 2, 0, 0 );
	sizer_build_picker->AddGrowableCol( 0 );
	sizer_build_picker->SetFlexibleDirection( wxBOTH );
	sizer_build_picker->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	text_build_path = new wxTextCtrl( panel_settings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	sizer_build_picker->Add( text_build_path, 0, wxALL|wxEXPAND, 5 );
	
	dir_picker_build = new wxDirPickerCtrl( panel_settings, DIR_PICKER_BUILD_ID, wxEmptyString, wxT("Choose build directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST );
	sizer_build_picker->Add( dir_picker_build, 0, wxALL|wxEXPAND, 5 );
	
	
	sizer_build->Add( sizer_build_picker, 1, wxEXPAND, 5 );
	
	
	sizer_settings->Add( sizer_build, 1, wxEXPAND, 5 );
	
	wxBoxSizer* sizer_start;
	sizer_start = new wxBoxSizer( wxVERTICAL );
	
	button_start = new wxButton( panel_settings, BUTTON_START_ID, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer_start->Add( button_start, 0, wxALL, 5 );
	
	button_stop = new wxButton( panel_settings, BUTTON_STOP_ID, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer_start->Add( button_stop, 0, wxALL, 5 );
	
	
	sizer_settings->Add( sizer_start, 1, wxEXPAND, 5 );
	
	
	panel_settings->SetSizer( sizer_settings );
	panel_settings->Layout();
	sizer_settings->Fit( panel_settings );
	sizer_main->Add( panel_settings, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( sizer_main );
	this->Layout();
	status_bar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
}

MainWindow::~MainWindow()
{
}
