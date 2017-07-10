#include "application.h"
#include "../memory/allocators.h"

#include "../logging/log.h"
#include "../logging/cvar.h"

#include <iostream>

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Application::Application(const size_t& max_memory) :
			running_(true),
			log_service_(nullptr)
		{
			Memory::Initialise<MallocAllocator>(max_memory);
		}

		//-----------------------------------------------------------------------------------------------
		Application::ExitCodes Application::Exec(const int& argc, char** argv)
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
		Application::~Application()
		{
			OnDestroy();
		}

		//-----------------------------------------------------------------------------------------------
		void Application::Initialise(const int& argc, char** argv)
		{
			cvar_service_ = Memory::ConstructUnique<CVar>();
			cvar_service_->ParseCommandLine(argc, argv);

			log_service_ = Memory::ConstructUnique<Log>();
			log_service_->Initialise();

			cvar_service_->LogAll();
			OnInit();
		}

		//-----------------------------------------------------------------------------------------------
		void Application::Shutdown()
		{
			OnShutdown();
			log_service_->Shutdown();
		}

		//-----------------------------------------------------------------------------------------------
		void Application::OnInit()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Application::OnStartup()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Application::OnUpdate()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Application::OnShutdown()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Application::OnDestroy()
		{

		}
	}
}