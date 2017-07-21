#include "application.h"
#include "../memory/allocators.h"

#include "../logging/logger.h"
#include "../logging/cvar.h"
#include "../io/content_manager.h"

#include <iostream>

#ifdef SNUFF_JAVASCRIPT
#include "../js/js_state_wrapper.h"
#include "../io/script.h"
#endif

#include "../core/window.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		SnuffboxApp::SnuffboxApp(const size_t& max_memory) :
			running_(true),
#ifdef SNUFF_JAVASCRIPT
			js_state_wrapper_(nullptr),
#endif
			log_service_(nullptr),
			cvar_service_(nullptr),
			window_service_(nullptr)
		{
			Memory::Initialise<MallocAllocator>(max_memory);
		}

		//-----------------------------------------------------------------------------------------------
		SnuffboxApp::ExitCodes SnuffboxApp::Exec(const int& argc, char** argv)
		{
			Initialise(argc, argv);

			OnStartup();

			while (window_service_->Closed() == false)
			{
				window_service_->Poll();
				content_service_->Update();
				OnUpdate();
			}

			Shutdown();

			return ExitCodes::kSuccess;
		}

		//-----------------------------------------------------------------------------------------------
		SnuffboxApp::~SnuffboxApp()
		{
			OnDestroy();
		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::Initialise(const int& argc, char** argv)
		{
			cvar_service_ = Memory::ConstructUnique<CVar>();
			log_service_ = Memory::ConstructUnique<Logger>();
			content_service_ = Memory::ConstructUnique<ContentManager>();
			window_service_ = Memory::ConstructUnique<Window>();

			cvar_service_->ParseCommandLine(argc, argv);
			log_service_->Initialise(cvar_service_.get());

			Services::Provide<CVarService>(cvar_service_.get());
			Services::Provide<LogService>(log_service_.get());

			content_service_->Initialise(cvar_service_.get());

			Services::Provide<ContentService>(content_service_.get());

			cvar_service_->LogAll();

			window_service_->Initialise("Snuffbox");
			Services::Provide<WindowService>(window_service_.get());

#ifdef SNUFF_JAVASCRIPT
			js_state_wrapper_ = Memory::ConstructUnique<JSStateWrapper>(Memory::default_allocator());
			js_state_wrapper_->Initialise();

			log_service_->Assert(content_service_->Load<Script>("main.js") != nullptr, 
							     "'main.js' is required in the current src_directory");
#endif

			log_service_->Log(console::LogSeverity::kSuccess, "Initialised the application");

			OnInit();
		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::Shutdown()
		{
			OnShutdown();

			log_service_->Log(console::LogSeverity::kInfo, "Shutting down..");

#ifdef SNUFF_JAVASCRIPT
			js_state_wrapper_->Shutdown();
#endif
			log_service_->Shutdown();
			cvar_service_->Shutdown();

			Services::Remove<CVarService>();
			Services::Remove<LogService>();
			Services::Remove<ContentService>();
			Services::Remove<WindowService>();
		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnInit()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnStartup()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnUpdate()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnShutdown()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnDestroy()
		{

		}
	}
}
