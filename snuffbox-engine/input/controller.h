#pragma once

#include "key_queue.h"
#include "buttons.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Controller : public snuffbox::engine::KeyQueue
		* @brief Used for controller input per connected controller
		* @author Daniel Konings
		*/
		class Controller : public KeyQueue
		{

			friend class Input;

		protected:

			/**
			* @brief Constructs a controller object with a specified ID
			* @param[in] id (unsigned int) The ID to assign to this controller
			*/
			Controller(unsigned int id);

			/**
			* @brief Polls the controller for input changes
			*/
			void Poll();

			/**
			* @brief Processes the current buttons and sends them to the key queue
			*/
			void ProcessButtons();

			/**
			* @brief Process the axes of the controller
			*/
			void ProcessAxes();

			/**
			* @brief Dead zones an axis value
			* @param[in] val (float) The value to dead zone
			* @return (float) The dead-zoned value
			*/
			float DeadZone(float val) const;

			/**
			* @brief Retrieves the value of a specified axis
			* @param[in] axis (snuffbox::engine::ControllerButtons::Axis) The axis to retrieve the value from
			* @return (float) The axis value
			*/
			float Axis(ControllerButtons::Axes axis) const;

			/**
			* @brief Called when the controller of this ID is connected
			*/
			void OnConnect();

			/**
			* @brief Called when the controller of this ID is disconnected
			*/
			void OnDisconnect();

			/**
			* @brief Sets the dead zone of this controller
			* @param[in] dz (float) The new value
			*/
			void set_dead_zone(float dz);

			/**
			* @return (bool) Is this controller connected?
			*/
			bool connected() const;

		private:

			unsigned int id_; //!< The ID of the controller
			float dead_zone_; //!< The dead zone of the controller
			bool connected_; //!< Is the controller at this ID connected?

			Vector<unsigned char> last_state_; //!< The last state of the buttons
			Vector<float> axes_; //!< The different axis values

			static const float DEFAULT_DEAD_ZONE_; //!< The default dead zone
		};
	}
}