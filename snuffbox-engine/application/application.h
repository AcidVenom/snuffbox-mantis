#pragma once

#include <snuffbox-logging/connection/logging_client.h>
#include <snuffbox-logging/logging_stream.h>

#include "../memory/memory.h"

#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		class Logger;
		class CVar;
		class ContentManager;

#ifdef SNUFF_JAVASCRIPT
		class JSStateWrapper;
		
		template <typename ... Args>
		class JSCallback;
#endif

		class Window;

		/**
		* @class snuffbox::engine::SnuffboxApp
		* @brief An application class to use as main entry point for the engine
		* @author Daniel Konings
		*/
		class SnuffboxApp
		{

			friend class ContentManager;

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
			* @param[in] max_memory (size_t) The maximum amount of memory for the application to use, default = 4Gb
			*/
			SnuffboxApp(size_t max_memory = static_cast<size_t>(4294967296));

			/**
			* @brief Runs the application
			* @param[in] argc (int) The number of command line arguments
			* @param[in] argv (char**) The command line arguments
			* @return (snuffbox::engine::SnuffboxApp::ExitCodes) The exit code of the application, 0 = success
			*/
			ExitCodes Exec(int argc, char** argv);

			/**
			* @brief Default destructor
			*/
			~SnuffboxApp();

		protected:

			/**
			* @brief Initialises the application
			* @param[in] argc (int) The number of command line arguments
			* @parma[in] argv (char**) The actual command line arguments
			*/
			void Initialise(int argc, char** argv);

			/**
			* @brief Called when a file is reloaded from the content manager
			* @param[in] path (const snuffbox::engine::String&) The relative path of the reloaded file
			*/
			void Reload(const String& path);

			/**
			* @brief Shuts the application down
			*/
			void Shutdown();

#ifdef SNUFF_JAVASCRIPT
			/**
			* @brief Binds the JavaScript callbacks to the current JavaScript context
			*/
			void BindJSCallbacks();
#endif

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
			* @param[in] dt (float) The current delta time of the application
			*/
			virtual void OnUpdate(float dt);

			/**
			* @brief Called when a file has been reloaded
			* @param[in] path (const snuffbox::engine::String&) The relative path to the reloaded file
			*/
			virtual void OnReload(const String& path);

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

			UniquePtr<JSCallback<>> js_on_startup_; //!< The JavaScript 'Application.onStartup(void)' callback
			UniquePtr<JSCallback<float>> js_on_update_; //!< The JavaScript 'Application.onUpdate(number)' callback
			UniquePtr<JSCallback<String>> js_on_reload_; //!< The JavaScript 'Application.onReload(string)' callback
			UniquePtr<JSCallback<>> js_on_shutdown_; //!< The JavaScript 'Application.onShutdown(void)' callback
#endif

			UniquePtr<Logger> log_service_; //!< The logging service
			UniquePtr<CVar> cvar_service_; //!< The CVar service
			UniquePtr<ContentManager> content_service_; //!< The content service
			UniquePtr<Window> window_service_; //!< The window to render to
		};
	}
}
