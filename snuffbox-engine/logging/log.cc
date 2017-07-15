#include "log.h" 
#include "cvar.h"

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
		Log::Log() :
			enabled_(true)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Log::Initialise(CVar* cvar)
		{
			CVarBoolean* cvconsole = cvar->Get<CVarBoolean>("console");

			if (cvconsole == nullptr || cvconsole->value() == false)
			{
				enabled_ = false;
				return;
			}

			CVarNumber* cvport = cvar->Get<CVarNumber>("console_port");
			CVarString* cvip = cvar->Get<CVarString>("console_ip");

			int port = cvport != nullptr ? cvport->As<int>() : SNUFF_DEFAULT_PORT;
			const char* ip = cvip != nullptr ? cvip->value().c_str() : "127.0.0.1";

			if (strcmp(ip, "localhost") == 0)
			{
				ip = "127.0.0.1";
			}

			if (cvport == nullptr || cvip == nullptr)
			{
				cvar->Set<CVarNumber>("console_port", static_cast<float>(port));
				cvar->Set<CVarString>("console_ip", ip);
			}

			stream_.Open(&client_, port, ip);
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Shutdown()
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Close();
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Debug(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kDebug, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Info(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kInfo, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Success(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kSuccess, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Warning(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kWarning, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Error(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kError, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::Fatal(const String& message)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kFatal, message.c_str(), static_cast<int>(message.size()));
		}

		//-----------------------------------------------------------------------------------------------
		void Log::RGB(const String& message, const console::LogColour& colour)
		{
			if (enabled_ == false)
			{
				return;
			}

			stream_.Log(console::LogSeverity::kRGB, message.c_str(), static_cast<int>(message.size()), &colour.background.r, &colour.foreground.r);
		}

		//-----------------------------------------------------------------------------------------------
		void Log::DoAssert(const bool& expr, const String& message)
		{
			if (expr == false)
			{
				Fatal("[ASSERTION FAILED]\n\t\t\t" + message);
				SNUFF_ASSERT();
			}
		}

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_SINGLE(Log, JS_BODY({
			JSFunctionRegister funcs[] =
			{
				JS_FUNCTION_REG(debug),
				JS_FUNCTION_REG(info),
				JS_FUNCTION_REG(success),
				JS_FUNCTION_REG(warning),
				JS_FUNCTION_REG(error),
				JS_FUNCTION_REG(fatal),
				JS_FUNCTION_REG(rgb),
				JS_FUNCTION_REG_END
			};

			JSFunctionRegister::Register(funcs, obj);
		}))

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Log, debug, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kDebug, wrapper.GetValue<String>(0, ""));
			}
		}))

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Log, info, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kInfo, wrapper.GetValue<String>(0, ""));
			}
		}))

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Log, success, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kSuccess, wrapper.GetValue<String>(0, ""));
			}
		}))

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Log, warning, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kWarning, wrapper.GetValue<String>(0, ""));
			}
		}))

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Log, error, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kError, wrapper.GetValue<String>(0, ""));
			}
		}))

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Log, fatal, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kFatal, wrapper.GetValue<String>(0, ""));
			}
		}))

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Log, rgb, JS_BODY({
			JSWrapper wrapper(args);
			if (wrapper.Check("S") == true)
			{
				console::LogColour col;

				col.background.r = static_cast<unsigned char>(wrapper.GetValue<float>(1, 0.0f));
				col.background.g = static_cast<unsigned char>(wrapper.GetValue<float>(2, 0.0f));
				col.background.b = static_cast<unsigned char>(wrapper.GetValue<float>(3, 0.0f));
				
				col.foreground.r = static_cast<unsigned char>(wrapper.GetValue<float>(4, 255.0f));
				col.foreground.g = static_cast<unsigned char>(wrapper.GetValue<float>(5, 255.0f));
				col.foreground.b = static_cast<unsigned char>(wrapper.GetValue<float>(6, 255.0f));

				Services::Get<LogService>().Log(console::LogSeverity::kRGB, wrapper.GetValue<String>(0, ""), col);
			}
		}))
	}
}