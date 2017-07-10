#include "app.h"
#include <cmath>
#include <snuffbox-engine/core/eastl.h>

#include <snuffbox-engine/services/log_service.h>

namespace snuffbox
{
	namespace test
	{
		//-----------------------------------------------------------------------------------------------
		App::App() :
			Application(static_cast<size_t>(4294967296))
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

			static float test = 0.0f;
			test += 0.1f;
			engine::String bleh = "BLEH";

			console::LogColour col
			{
				{230, 250, 0},
				{0, 0, 0}
			};

			log.Log(console::LogSeverity::kRGB, "Update {0}, {1}, {0}", test, bleh.c_str(), col);
		}

		//-----------------------------------------------------------------------------------------------
		void App::OnShutdown()
		{
			engine::Services::Get<engine::LogService>().Log(console::LogSeverity::kInfo, "Shutting down..");
		}
	}
}
