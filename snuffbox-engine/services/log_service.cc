#include "log_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		LogService::LogService()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Debug(const String& message)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Info(const String& message)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Success(const String& message)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Warning(const String& message)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Error(const String& message)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Fatal(const String& message)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::RGB(const String& message, const console::LogColour& colour)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LogService::Assert(const bool& expr, const String& message)
		{
			assert(expr);
		}
	}
}