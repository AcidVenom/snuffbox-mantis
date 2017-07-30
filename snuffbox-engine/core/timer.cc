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
				Reset();
			}

			start_ = std::chrono::high_resolution_clock::now();
			started_ = true;
		}

		//-----------------------------------------------------------------------------------------------
		float Timer::Stop(Unit unit, bool echo)
		{
			if (started_ == false)
			{
				return Convert(elapsed_, unit);
			}

			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
			float elapsed = std::chrono::duration<float>(now - start_).count();

			start_ = now;

			elapsed_ += elapsed;

			started_ = false;

			elapsed = Convert(elapsed_, unit);

			if (echo == true)
			{
				String type;

				switch (unit)
				{
				case Unit::kMilliseconds:
					type = "ms";
					break;

				case Unit::kSeconds:
					type = " second(s)";
					break;

				case Unit::kMinutes:
					type = " minute(s)";
					break;
				}

				Services::Get<LogService>().Log(console::LogSeverity::kDebug, "'{0}' ran for: {1}{2}", name_, elapsed, type);
			}

			return elapsed;
		}

		//-----------------------------------------------------------------------------------------------
		float Timer::Elapsed(Unit unit)
		{
			if (started_ == false)
			{
				return Convert(elapsed_, unit);
			}

			float elapsed = Stop(unit, false);
			Start(false);

			return elapsed;
		}

		//-----------------------------------------------------------------------------------------------
		void Timer::Reset()
		{
			elapsed_ = 0.0f;
		}

		//-----------------------------------------------------------------------------------------------
		float Timer::Convert(float elapsed, Unit unit)
		{
			switch (unit)
			{
			case Unit::kMilliseconds:
				return elapsed * 1e3f;

			case Unit::kSeconds:
				return elapsed;

			case Unit::kMinutes:
				return elapsed / 60.0f;
			}

			return elapsed;
		}

		//-----------------------------------------------------------------------------------------------
		JS_CONSTRUCTOR(Timer, JS_BODY(
		{
			JSWrapper wrapper(args);
			
			name_ = wrapper.GetValue<String>(0, "");
			started_ = false;
			elapsed_ = 0.0f;
			start_ = std::chrono::high_resolution_clock::now();
		}));

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_TMPL(Timer, JS_BODY(
		{
			JSFunctionRegister funcs[] =
			{
				JS_FUNCTION_REG(start),
				JS_FUNCTION_REG(stop),
				JS_FUNCTION_REG(elapsed),
				JS_FUNCTION_REG(reset),
				JS_FUNCTION_REG_END
			};

			JSWrapper::SetFunctionTemplateValue<int>(func, "Milliseconds", static_cast<int>(Unit::kMilliseconds));
			JSWrapper::SetFunctionTemplateValue<int>(func, "Seconds", static_cast<int>(Unit::kSeconds));
			JSWrapper::SetFunctionTemplateValue<int>(func, "Minutes", static_cast<int>(Unit::kMinutes));

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Timer, start, JS_BODY(
		{
			JS_SETUP(Timer);

			self->Start(wrapper.GetValue<bool>(0, true));
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Timer, stop, JS_BODY(
		{
			JS_SETUP(Timer);

			float elapsed = self->Stop(
				static_cast<Unit>(wrapper.GetValue<int>(0, static_cast<int>(Unit::kMilliseconds))), 
				wrapper.GetValue<bool>(1, false));

			wrapper.ReturnValue<float>(elapsed);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Timer, elapsed, JS_BODY(
		{
			JS_SETUP(Timer);

			float elapsed = self->Elapsed(
				static_cast<Unit>(wrapper.GetValue<int>(0, static_cast<int>(Unit::kMilliseconds))));

			wrapper.ReturnValue<float>(elapsed);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Timer, reset, JS_BODY(
		{
			JS_SETUP(Timer);

			self->Reset();
		}));
	}
}