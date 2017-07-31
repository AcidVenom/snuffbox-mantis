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

			/**
			* @brief Retrieves the mouse position in the client area
			* @param[out] px (int*) The x coordinate, in pixels
			* @param[out] py (int*) The y coordinate, in pixels
			*/
			void MousePosition(int* px, int* py) const;

			/**
			* @brief Retrieves the mouse movement of this frame
			* @param[out] mx (int*) The x movement, in pixels
			* @param[out] my (int*) The y movement, in pixels
			*/
			void MouseMovement(int* mx, int* my) const;

			/**
			* @brief Sets the mouse position
			* @param[in] x (int) The x coordinate, in pixels
			* @param[in] y (int) The y coordinate, in pixels
			*/
			void SetPosition(int x, int y);

			/**
			* @brief Updates the mouse
			* @return (bool) Was the mouse used this frame?
			*/
			bool Update();

		private:

			int position_[2]; //!< The current position of the mouse within the client area
			int delta_[2]; //!< The current movement of the mouse
		};
	}
}