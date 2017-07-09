#include "application.h"
#include "../memory/allocators.h"

#include "../logging/log.h"

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
			Initialise();

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
		void Application::Initialise()
		{
			log_service_ = Memory::ConstructUnique<Log>();
			log_service_->Initialise();
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