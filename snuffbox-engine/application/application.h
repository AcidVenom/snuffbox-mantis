#pragma once

#include <snuffbox-logging/connection/logging_client.h>
#include <snuffbox-logging/logging_stream.h>

#include "../memory/memory.h"

#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		class Log;
		class CVar;
		class ContentManager;

#ifdef SNUFF_JAVASCRIPT
		class JSStateWrapper;
#endif

		class Window;

		/**
		* @class snuffbox::engine::SnuffboxApp
		* @brief An application class to use as main entry point for the engine
		* @author Daniël Konings
		*/
		class SnuffboxApp
		{

		public:

			/**
			* @brief The different exit codes of the application
			*/
			enum ExitCodes : int
			{
				kSuccess, //!< When the application ran and executed succesfully
				kUnknown //!< When an unknown error has occurred
			};

			/**
			* @brief Default constructor
			* @param[in] max_memory (const size_t&) The maximum amount of memory for the application to use, default = 4Gb
			*/
			SnuffboxApp(const size_t& max_memory = static_cast<size_t>(4294967296));

			/**
			* @brief Runs the application
			* @param[in] argc (const int&) The number of command line arguments
			* @param[in] argv (char**) The command line arguments
			* @return (snuffbox::engine::SnuffboxApp::ExitCodes) The exit code of the application, 0 = success
			*/
			ExitCodes Exec(const int& argc, char** argv);

			/**
			* @brief Default destructor
			*/
			~SnuffboxApp();

		protected:

			/**
			* @brief Initialises the application
			* @param[in] argc (const int&) The number of command line arguments
			* @parma[in] argv (char**) The actual command line arguments
			*/
			void Initialise(const int& argc, char** argv);

			/**
			* @brief Shuts the application down
			*/
			void Shutdown();

			/**
			* @brief Called after initialisation of the base application
			*/
			virtual void OnInit();

			/**
			* @brief Called after startup of the base application
			* @see snuffbox::engine::SnuffboxApp::Exec
			*/
			virtual void OnStartup();

			/**
			* @brief Called each frame when the application updates
			*/
			virtual void OnUpdate();

			/**
			* @brief Called when the application is shutdown, before destruction
			*/
			virtual void OnShutdown();

			/**
			* @brief Called when the application is destructed
			*/
			virtual void OnDestroy();

		private:

			bool running_; //!< Is the application still running?

#ifdef SNUFF_JAVASCRIPT
			UniquePtr<JSStateWrapper> js_state_wrapper_; //!< The JavaScript state wrapper
#endif

			UniquePtr<Log> log_service_; //!< The logging service
			UniquePtr<CVar> cvar_service_; //!< The CVar service
			UniquePtr<ContentManager> content_service_; //!< The content service
			UniquePtr<Window> window_service_; //!< The window to render to
		};
	}
}
