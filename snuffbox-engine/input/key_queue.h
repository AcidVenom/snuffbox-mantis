#pragma once

#include "../input/keycodes.h"
#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		class Input;

		/**
		* @class snuffbox::engine::KeyQueue
		* @brief Used to queue key and button events and change their states accordingly
		* @author Daniel Konings
		*/
		class KeyQueue
		{

			friend class Input;

		protected:

			/**
			* @struct snuffbox::engine::KeyQueue::Event
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
			* @brief Constructs the key queue with a specified size
			* @param[in] size (snuffbox::engine::KeyCodes::KeyCode) The size of the key queue
			* @param[in] any (snuffbox::engine::KeyCodes::KeyCode) The keycode for the 'any' key
			*/
			KeyQueue(KeyCodes::KeyCode size, KeyCodes::KeyCode any);

			/**
			* @brief Queues a key or event
			* @param[in] evt (const snuffbox::engine::KeyQueue::Event&) The event to queue
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
			* @return (unsigned int) The number of processed events
			*/
			unsigned int Flush();

			/**
			* @brief Is a specified key or button in a specified state this frame?
			* @param[in] key (snuffbox::engine::KeyCodes::KeyCode) The keycode to check for
			* @return (bool) Was it in the specified state?
			*/
			bool HasKeyState(KeyCodes::KeyCode key, KeyState state) const;

			/**
			* @brief Checks if any key is in a specified state
			* @param[in] state (snuffbox::engine::KeyQueue::KeyState) The state to check for
			* @return (bool) Was any key in the specified state?
			*/
			bool AnyKey(KeyState state) const;

			/**
			* @return (snuffbox::engine::KeyCodes::KeyCode) The last key or button that was in the specified state
			*/
			KeyCodes::KeyCode Last(KeyState state) const;

		private:

			KeyCodes::KeyCode any_; //!< The keycode for the 'any' key

			Queue<Event> queue_; //!< The event queue
			Vector<KeyState> states_; //!< The different keystates
			KeyCodes::KeyCode last_[static_cast<int>(KeyState::kCount)]; //!< The last key per key state that was received as input
		};
	}
}