#pragma once

#include <snuffbox-logging/connection/logging_client.h>
#include <snuffbox-logging/logging_stream.h>

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Application
		* @brief An application class to use as main entry point for the engine
		* @author Daniël Konings
		*/
		class Application
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
			* @param[in] max_memory (const size_t&) The maximum amount of memory for the application to use
			*/
			Application(const size_t& max_memory);

			/**
			* @brief Runs the application
			* @param[in] argc (const int&) The number of command line arguments
			* @param[in] argv (char**) The command line arguments
			* @return (snuffbox::engine::Application::ExitCodes) The exit code of the application, 0 = success
			*/
			ExitCodes Exec(const int& argc, char** argv);

			/**
			* @brief Default destructor
			*/
			~Application();

		protected:

			/**
			* @brief Initialises the application
			*/
			void Initialise();

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
			* @see snuffbox::engine::Application::Exec
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

		protected:

			logging::LoggingStream stream_; //!< The logging stream
			logging::LoggingClient client_; //!< The logging client
		};
	}
}