#pragma once

#include "key_queue.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Keyboard : public snuffbox::engine::KeyQueue
		* @brief The helper class for keyboard input and to handle all the keyboard related events
		* @author Daniel Konings
		*/
		class Keyboard : public KeyQueue
		{
			friend class Input;

		protected:

			/**
			* @brief Initialises the key queue
			*/
			Keyboard();
		};
	}
}
