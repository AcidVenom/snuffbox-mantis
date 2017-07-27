#include "timer.h"
#include "../services/log_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Timer::Timer(const String& name) :
			name_(name),
			started_(false),
			elapsed_(0.0f),
			start_(std::chrono::high_resolution_clock::now())
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Timer::Start(bool reset)
		{
			if (reset == true)
			{
				elapsed_ = 0.0f;
			}

			start_ = std::chrono::high_resolution_clock::now();
			started_ = true;
		}

		//-----------------------------------------------------------------------------------------------
		float Timer::Stop(Unit unit, bool echo)
		{
			if (started_ == false)
			{
				return Elapsed(unit);
			}

			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
			float elapsed = std::chrono::duration<float>(now - start_).count();

			start_ = now;

			if (echo == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kDebug, "'{0}' ran for: {1}ms", name_, static_cast<int>(elapsed * 1e3f));
			}

			elapsed_ += elapsed;

			started_ = false;

			return Elapsed(unit);
		}

		//-----------------------------------------------------------------------------------------------
		float Timer::Elapsed(Unit unit) const
		{
			switch (unit)
			{
			case Unit::kMilliseconds:
				return elapsed_ * 1e3f;

			case Unit::kSeconds:
				return elapsed_;

			case Unit::kMinutes:
				return elapsed_ / 60.0f;
			}

			return elapsed_;
		}

		//-----------------------------------------------------------------------------------------------
		JS_CONSTRUCTOR(Timer, JS_BODY({
			
			JSWrapper wrapper(args);
			
			name_ = wrapper.GetValue<String>(0, "");
			started_ = false;
			elapsed_ = 0.0f;
			start_ = std::chrono::high_resolution_clock::now();
		}));

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_TMPL(Timer, JS_BODY({

			JSFunctionRegister funcs[] =
			{
				JS_FUNCTION_REG(start),
				JS_FUNCTION_REG(stop),
				JS_FUNCTION_REG_END
			};

			JSWrapper::SetFunctionTemplateValue<int>(func, "Milliseconds", 0);
			JSWrapper::SetFunctionTemplateValue<int>(func, "Seconds", 1);
			JSWrapper::SetFunctionTemplateValue<int>(func, "Minutes", 2);

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Timer, start, JS_BODY({
			
			JS_SETUP(Timer);

			self->Start(wrapper.GetValue<bool>(0, true));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Timer, stop, JS_BODY({
			
			JS_SETUP(Timer);

			float elapsed = self->Stop(static_cast<Unit>(wrapper.GetValue<int>(0, 1)), wrapper.GetValue<bool>(1, false));

			wrapper.ReturnValue<float>(elapsed);
		}));
	}
}