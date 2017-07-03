#pragma once

#include "../defines.h"

namespace snuffbox
{
	class Console;

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

		Console* console_; //!< The main console window of the application generated through a form
	};
}