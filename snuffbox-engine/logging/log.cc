#include "log.h" 

#ifndef SNUFF_DEBUG
	#ifdef SNUFF_WIN32
		#define SNUFF_ASSERT()	\
		while (true)			\
		{						\
			__debugbreak();		\
		}
	#elif SNUFF_LINUX
		#include <signal.h>
		#define SNUFF_ASSERT()	\
		while (true)			\
		{						\
			raise(SIGTRAP);		\
		}
	#endif
#else
	#define SNUFF_ASSERT() assert(false)
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Log::Log()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Log::Initialise()
		{
			stream_.Open(&client_);
			Services::Provide<LogService>(this);
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Shutdown()
		{
			stream_.Close();
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Debug(const String& message)
		{
			stream_.Log(console::LogSeverity::kDebug, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Info(const String& message)
		{
			stream_.Log(console::LogSeverity::kInfo, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Success(const String& message)
		{
			stream_.Log(console::LogSeverity::kSuccess, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Warning(const String& message)
		{
			stream_.Log(console::LogSeverity::kWarning, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Error(const String& message)
		{
			stream_.Log(console::LogSeverity::kError, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Fatal(const String& message)
		{
			stream_.Log(console::LogSeverity::kFatal, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::RGB(const String& message, const console::LogColour& colour)
		{
			stream_.Log(console::LogSeverity::kRGB, message.c_str(), static_cast<int>(message.size()), &colour.background.r, &colour.foreground.r);
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Assert(const bool& expr, const String& message)
		{
			if (expr == false)
			{
				Fatal("[ASSERTION FAILED]\n\t\t\t" + message);
				SNUFF_ASSERT();
			}
		}

		//-----------------------------------------------------------------------------------------------
		Log::~Log()
		{
			Services::Remove<LogService>();
		}
	}
}