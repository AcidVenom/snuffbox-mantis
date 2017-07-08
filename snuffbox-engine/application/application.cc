#include "application.h"
#include "../memory/memory.h"
#include "../memory/allocators.h"
#include <iostream>

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Application::Application(const size_t& max_memory) :
			running_(true)
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
			stream_.Close();
		}

		//-----------------------------------------------------------------------------------------------
		void Application::Initialise()
		{
			stream_.Open(&client_);
			OnInit();
		}

		//-----------------------------------------------------------------------------------------------
		void Application::Shutdown()
		{
			OnShutdown();
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