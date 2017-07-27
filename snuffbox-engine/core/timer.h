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
        class Timer : JS_OBJECT
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
			* @remarks This automatically runs the timer
			*/
            Timer(const String& name);
			
			/**
			* @brief Restarts the timer
			* @param[in] reset (bool) Should the timer be reset? default = true
			*/
			void Start(bool reset = true);

			/**
			* @brief Stops the timer
			* @param[in] unit (snuffbox::engine::Timer::Unit) The unit to represent the time value in, default = snuffbox::engine::Timer::unit::kMilliseconds
			* @param[in] echo (bool) Should the timer log its result? default = false
			* @return (float) The time this timer has ran in milliseconds
			*/
			float Stop(Unit unit = Unit::kMilliseconds, bool echo = false);

		protected:

			/**
			* @brief Converts the elapsed time into a specific unit
			* @param[in] unit (snuffbox::engine::Timer::Unit) The unit to represent the time value in, default = snuffbox::engine::Timer::Unit::kMilliseconds
			* @return (float) The time that has elapsed since the start time in the provided unit
			*/
			float Elapsed(Unit unit = Unit::kMilliseconds) const;

		private:

			String name_; //!< The name of this timer

			bool started_; //!< Has the timer been started?
			float elapsed_; //!< The elapsed time between starts and stops
			std::chrono::high_resolution_clock::time_point start_; //!< The start timepoint

		public:

			JS_NAME_TMPL(Timer);

			JS_FUNCTION_DECL(start);
			JS_FUNCTION_DECL(stop);
        };
    }
}
