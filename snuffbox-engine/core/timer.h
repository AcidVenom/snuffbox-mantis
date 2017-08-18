#pragma once

#include "eastl.h"
#include <chrono>

#include "../js/js_defines.h"

namespace snuffbox
{
    namespace engine
    {
		/**
		* @class snuffbox::engine::Timer : [JSObject]
		* @brief Used for profiling, time based events and delta timing
		* @author Daniel Konings
		*/
        class Timer JS_OBJECT
        {

        public:

			/**
			* @brief Used to represent the current time value as certain time unit
			*/
			enum struct Unit
			{
				kMilliseconds, //!< Milliseconds, seconds * 1e3
				kSeconds, //!< Seconds, the default
				kMinutes //!< Minutes, seconds divided by 60
			};

			/**
			* @brief Construct with a name
			* @param[in] name (const snuffbox::engine::String&) The name to construct this timer with
			* @param[in] auto_start (bool) Should the timer be automatically started? Default = true
			*/
            Timer(const String& name, bool auto_start = true);
			
			/**
			* @brief Restarts the timer
			* @param[in] reset (bool) Should the timer be reset? Default = true
			*/
			void Start(bool reset = true);

			/**
			* @brief Stops the timer
			* @param[in] unit (snuffbox::engine::Timer::Unit) The unit to represent the time value in, default = snuffbox::engine::Timer::Unit::kMilliseconds
			* @param[in] echo (bool) Should the timer log its result? default = false
			* @return (float) The time this timer has ran in the converted unit
			*/
			float Stop(Unit unit = Unit::kMilliseconds, bool echo = false);

			/**
			* @brief Gets the current elapsed time
			* @param[in] unit (snuffbox::engine::Timer::Unit) The unit to represent the time value in, default = snuffbox::engine::Timer::Unit::kMilliseconds
			* @return (float) The time this timer has ran in the converted unit
			*/
			float Elapsed(Unit unit = Unit::kMilliseconds);

			/**
			* @brief Resets the timer
			*/
			void Reset();

		protected:

			/**
			* @brief Converts a float time into a specific unit
			* @param[in] elapsed (float) The elapsed time
			* @param[in] unit (snuffbox::engine::Timer::Unit) The unit to represent the time value in
			* @return (float) The converted time
			*/
			static float Convert(float elapsed, Unit unit);

		private:

			String name_; //!< The name of this timer

			bool started_; //!< Has the timer been started?
			float elapsed_; //!< The elapsed time between starts and stops
			std::chrono::high_resolution_clock::time_point start_; //!< The start timepoint

		public:

			JS_NAME_TMPL(Timer);

			JS_FUNCTION_DECL(start);
			JS_FUNCTION_DECL(stop);
			JS_FUNCTION_DECL(elapsed);
			JS_FUNCTION_DECL(reset);
        };
    }
}
