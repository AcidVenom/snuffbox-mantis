#include "application.h"
#include "../memory/allocators.h"

#include "../logging/log.h"
#include "../logging/cvar.h"

#include <iostream>

#ifdef SNUFF_JAVASCRIPT
#include "../js/js_state_wrapper.h"
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		SnuffboxApp::SnuffboxApp(const size_t& max_memory) :
			running_(true),
			log_service_(nullptr)
		{
			Memory::Initialise<MallocAllocator>(max_memory);
		}

		//-----------------------------------------------------------------------------------------------
		SnuffboxApp::ExitCodes SnuffboxApp::Exec(const int& argc, char** argv)
		{
			Initialise(argc, argv);

			OnStartup();

			while (std::cin.get() != 'q')
			{
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
			log_service_ = Memory::ConstructUnique<Log>();

			cvar_service_->ParseCommandLine(argc, argv);
			log_service_->Initialise(cvar_service_.get());

			Services::Provide<CVarService>(cvar_service_.get());
			Services::Provide<LogService>(log_service_.get());

#ifdef SNUFF_JAVASCRIPT
			js_state_wrapper_ = Memory::ConstructUnique<JSStateWrapper>(Memory::default_allocator());
			js_state_wrapper_->Initialise();
#endif

			cvar_service_->LogAll();

			OnInit();
		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::Shutdown()
		{
			OnShutdown();

			js_state_wrapper_->Shutdown();
			log_service_->Shutdown();

			Services::Remove<CVarService>();
			Services::Remove<LogService>();
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