#pragma once

#include "../wx_defines.h"

namespace snuffbox
{
	namespace console
	{
		class Console;

		/**
		* @class snuffbox::console::ConsoleApp : public wxApp
		* @brief The wxWidgets application that will contain all the wx windows
		* @author Daniel Konings
		*/
		class ConsoleApp : public wxApp
		{
		public:

			/**
			* @brief Called when the wxApp is initialised
			* @return (bool) Was the initialisation a success?
			*/
			bool OnInit() override;

			/**
			* @brief Called when the wxApp is closed
			* @return (int) The exit code
			*/
			int OnExit() override;

            /**
            * @brief Called when the wxApp receives an event
            * @param[in] evt (wxEvent&) The received event
            * @return (int) -1 if the event was skipped for this filter
            */
            int FilterEvent(wxEvent& evt) override;

		private:

			Console* console_; //!< The main console window of the application generated through a form
		};
	}
}
