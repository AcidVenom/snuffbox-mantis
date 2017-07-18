#include "app.h"
#include <cmath>
#include <snuffbox-engine/core/eastl.h>

#include <snuffbox-engine/services/log_service.h>
#include <snuffbox-engine/services/cvar_service.h>

namespace snuffbox
{
	namespace test
	{
		//-----------------------------------------------------------------------------------------------
		App::App()
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		void App::OnInit()
		{
			engine::Services::Get<engine::LogService>().Log(console::LogSeverity::kSuccess, "Initialised the application");
		}

		//-----------------------------------------------------------------------------------------------
		void App::OnUpdate()
		{
			engine::LogService& log = engine::Services::Get<engine::LogService>();
			engine::CVarService& cvar = engine::Services::Get<engine::CVarService>();
			
			log.Log(console::LogSeverity::kDebug, "Update");

			std::this_thread::sleep_for(std::chrono::milliseconds(4));
		}

		//-----------------------------------------------------------------------------------------------
		void App::OnShutdown()
		{
			engine::Services::Get<engine::LogService>().Log(console::LogSeverity::kInfo, "Shutting down..");
		}
	}
}
