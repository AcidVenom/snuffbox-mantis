#include "application.h"
#include "../memory/allocators.h"

#include "../logging/logger.h"
#include "../logging/cvar.h"
#include "../io/content_manager.h"

#include <iostream>

#ifdef SNUFF_JAVASCRIPT
#include "../js/js_state_wrapper.h"
#include "../js/js_callback.h"
#include "../io/script.h"
#endif

#include "../core/window.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		SnuffboxApp::SnuffboxApp(size_t max_memory) :
			running_(true),
#ifdef SNUFF_JAVASCRIPT
			js_state_wrapper_(nullptr),
			js_on_startup_(nullptr),
			js_on_update_(nullptr),
			js_on_reload_(nullptr),
			js_on_shutdown_(nullptr),
#endif
			log_service_(nullptr),
			cvar_service_(nullptr),
			content_service_(nullptr),
			window_service_(nullptr),
			delta_timer_(nullptr),
			delta_time_(0.0f)
		{
			Memory::Initialise<MallocAllocator>(max_memory);
		}

		//-----------------------------------------------------------------------------------------------
		SnuffboxApp::ExitCodes SnuffboxApp::Exec(int argc, char** argv)
		{
			Initialise(argc, argv);

			OnStartup();

#ifdef SNUFF_JAVASCRIPT
			js_on_startup_->Call();
#endif
			log_service_->client_.IdleNotification();

			while (window_service_->Closed() == false)
			{
				delta_timer_->Start();
				window_service_->Poll();
				content_service_->Update();

				OnUpdate(delta_time_);

#ifdef SNUFF_JAVASCRIPT
				js_on_update_->Call(delta_time_);
#endif
				log_service_->client_.IdleNotification();
				delta_time_ = delta_timer_->Stop(Timer::Unit::kSeconds);
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
		void SnuffboxApp::Initialise(int argc, char** argv)
		{
			Timer init_time("Initialisation");
			init_time.Start();
			cvar_service_ = Memory::ConstructUnique<CVar>();
			log_service_ = Memory::ConstructUnique<Logger>();
			content_service_ = Memory::ConstructUnique<ContentManager>();
			window_service_ = Memory::ConstructUnique<Window>();

			cvar_service_->ParseCommandLine(argc, argv);
			log_service_->Initialise(cvar_service_.get());

			Services::Provide<CVarService>(cvar_service_.get());
			Services::Provide<LogService>(log_service_.get());

			content_service_->Initialise(cvar_service_.get(), this);

			Services::Provide<ContentService>(content_service_.get());

			cvar_service_->LogAll();

			window_service_->Initialise("Snuffbox");
			Services::Provide<WindowService>(window_service_.get());

			delta_timer_ = Memory::ConstructUnique<Timer>("Delta time");

#ifdef SNUFF_JAVASCRIPT
			js_state_wrapper_ = Memory::ConstructUnique<JSStateWrapper>(Memory::default_allocator());
			js_state_wrapper_->Initialise();

			log_service_->Assert(content_service_->Load<Script>("main.js") != nullptr, 
							     "'main.js' is required in the current src_directory");

			js_on_startup_ = Memory::ConstructUnique<JSCallback<>>();
			js_on_update_ = Memory::ConstructUnique<JSCallback<float>>();
			js_on_reload_ = Memory::ConstructUnique<JSCallback<String>>();
			js_on_shutdown_ = Memory::ConstructUnique<JSCallback<>>();

			BindJSCallbacks();
#endif
			init_time.Stop(Timer::Unit::kMilliseconds, true);
			log_service_->Log(console::LogSeverity::kSuccess, "Initialised the application");

			OnInit();
		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::Reload(const String& path)
		{
#ifdef SNUFF_JAVASCRIPT
			BindJSCallbacks();

			js_on_reload_->Call(path);
#endif
			OnReload(path);
		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::Shutdown()
		{
			OnShutdown();

#ifdef SNUFF_JAVASCRIPT
			js_on_shutdown_->Call();
#endif

			log_service_->Log(console::LogSeverity::kInfo, "Shutting down..");

#ifdef SNUFF_JAVASCRIPT
			js_on_startup_->Clear();
			js_on_update_->Clear();
			js_on_reload_->Clear();
			js_on_shutdown_->Clear();

			js_state_wrapper_->Shutdown();
#endif
			log_service_->client_.IdleNotification();

			log_service_->Shutdown();
			cvar_service_->Shutdown();

			Services::Remove<CVarService>();
			Services::Remove<LogService>();
			Services::Remove<ContentService>();
			Services::Remove<WindowService>();
		}

#ifdef SNUFF_JAVASCRIPT
		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::BindJSCallbacks()
		{
			js_on_startup_->Set("Application", "onStartup");
			js_on_update_->Set("Application", "onUpdate");
			js_on_reload_->Set("Application", "onReload");
			js_on_shutdown_->Set("Application", "onShutdown");
		}
#endif

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnInit()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnStartup()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnUpdate(float dt)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void SnuffboxApp::OnReload(const String& path)
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
