#pragma once

#include "../defines.h"

namespace snuffbox
{
	class MainWindow;

	/**
	* @class snuffbox::ConsoleApp : public wxApp
	* @brief The wxWidgets application that will contain all the wx windows
	* @author Daniël Konings
	*/
	class ConsoleApp : public wxApp
	{
	public:

		/**
		* @brief Called when the wxApp is initialised
		* @return (bool) Was the initialisation a success?
		*/
		bool OnInit() override;

	private:

		MainWindow* main_window_; //!< The main window of the application generated through a form
	};
}