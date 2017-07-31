#pragma once

#include "key_queue.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Mouse : public snuffbox::engine::KeyQueue
		* @brief The helper class for mouse input and to handle all the mouse related events
		* @author Daniel Konings
		*/
		class Mouse : public KeyQueue
		{
			friend class Input;

		protected:

			/**
			* @brief Initialises the key queue
			*/
			Mouse();
		};
	}
}