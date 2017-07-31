#pragma once

#include "../core/eastl.h"
#include "../input/keycodes.h"

namespace snuffbox
{
	namespace engine
	{
		class Input;

		/**
		* @class snuffbox::engine::Keyboard
		* @brief The helper class for keyboard input and to handle all the keyboard related events
		* @author Daniel Konings
		*/
		class Keyboard
		{
			friend class Input;

		protected:

			/**
			* @brief Protected constructor
			*/
			Keyboard();

			/**
			* @struct snuffbox::engine::Keyboard::Event
			* @brief An event structure to pass events to the keyboard
			* @author Daniel Konings
			*/
			struct Event
			{
				/**
				* @brief The different event types
				*/
				enum struct Type
				{
					kPressed, //!< When a key was pressed
					kRepeat, //!< When a key was repeated
					kReleased //!< When a key was released
				};

				Type type; //!< The type of event received
				KeyCodes::KeyCode key; //!< The corresponding key related to this event
			};
			
			/**
			* @brief The different available keystates a key can have
			*/
			enum struct KeyState
			{
				kPressed, //!< When a key was pressed last frame
				kDown, //!< When a key was down last frame
				kReleased, //!< When a key was released last frame
				kUp, //!< When nothing has happened to the key last frame
				kCount //!< The number of key states
			};

			/**
			* @brief Queues an event to the keyboard
			*/
			void PostEvent(const Event& evt);

			/**
			* @brief Called when a pressed event is processed
			* @param[in] key (snuffbox::engine::KeyCodes::KeyCode) The corresponding keycode
			*/
			void OnPressed(KeyCodes::KeyCode key);

			/**
			* @brief Called when a released event is processed
			* @param[in] key (snuffbox::engine::KeyCodes::KeyCode) The corresponding keycode
			*/
			void OnReleased(KeyCodes::KeyCode key);

			/**
			* @brief Resets all states for this frame to the 'down/up' state, before handling new events
			*/
			void ResetStates();

			/**
			* @brief Processes the event queue
			*/
			void Flush();

		private:

			static const KeyCodes::KeyCode KEY_LENGTH_ = KeyCodes::kNone + 1; //!< The total length of the key array

			Queue<Event> queue_; //!< The event queue
			KeyState states_[KEY_LENGTH_]; //!< The different keystates
			KeyCodes::KeyCode last_[static_cast<int>(KeyState::kCount)]; //!< The last key per key state that was received as input
		};
	}
}