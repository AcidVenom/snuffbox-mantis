#include "key_queue.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		KeyQueue::KeyQueue(KeyCodes::KeyCode size, KeyCodes::KeyCode any) :
			any_(any)
		{
			states_.resize(size);

			for (KeyCodes::KeyCode i = 0; i < size; ++i)
			{
				states_[i] = KeyState::kUp;
			}

			for (int i = 0; i < static_cast<int>(KeyState::kCount); ++i)
			{
				last_[i] = size - 1;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void KeyQueue::PostEvent(const KeyQueue::Event& evt)
		{
			queue_.push(evt);
		}

		//-----------------------------------------------------------------------------------------------
		void KeyQueue::OnPressed(KeyCodes::KeyCode key)
		{
			if (key >= states_.size())
			{
				return;
			}

			states_.at(key) = KeyState::kPressed;
		}

		//-----------------------------------------------------------------------------------------------
		void KeyQueue::OnReleased(KeyCodes::KeyCode key)
		{
			if (key >= states_.size())
			{
				return;
			}

			states_.at(key) = KeyState::kReleased;
		}

		//-----------------------------------------------------------------------------------------------
		void KeyQueue::ResetStates()
		{
			for (KeyCodes::KeyCode i = 0; i < static_cast<KeyCodes::KeyCode>(states_.size()); ++i)
			{
				KeyState& state = states_.at(i);
				state = (state == KeyState::kPressed || state == KeyState::kDown) ? KeyState::kDown : KeyState::kUp;
			}
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int KeyQueue::Flush()
		{
			unsigned int count = 0;

			ResetStates();

			KeyCodes::KeyCode key;
			KeyState state;

			while (queue_.empty() == false)
			{
				const KeyQueue::Event& front = queue_.front();
				key = front.key;

				if (key >= states_.size())
				{
					queue_.pop();
					continue;
				}

				switch (front.type)
				{
				case Event::Type::kPressed:
					OnPressed(key);
					break;

				case Event::Type::kRepeat:
					break;

				case Event::Type::kReleased:
					OnReleased(key);
					break;
				}

				state = states_[key];

				if (state == KeyState::kPressed)
				{
					last_[static_cast<int>(KeyState::kDown)] = key;
				}

				last_[static_cast<int>(state)] = key;

				queue_.pop();

				++count;
			}

			return count;
		}

		//-----------------------------------------------------------------------------------------------
		bool KeyQueue::HasKeyState(KeyCodes::KeyCode key, KeyState state) const
		{
			if (key >= states_.size())
			{
				return false;
			}

			if (key == any_)
			{
				return AnyKey(state);
			}

			const KeyState& key_state = states_.at(key);
			return key_state == state || (state == KeyState::kDown && key_state == KeyState::kPressed);
		}

		//-----------------------------------------------------------------------------------------------
		bool KeyQueue::AnyKey(KeyState state) const
		{
			for (KeyCodes::KeyCode i = 0; i < static_cast<KeyCodes::KeyCode>(states_.size()); ++i)
			{
				const KeyState& key = states_.at(i);
				if (key == state || (state == KeyState::kDown && key == KeyState::kPressed))
				{
					return true;
				}
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		KeyCodes::KeyCode KeyQueue::Last(KeyState state) const
		{
			return last_[static_cast<int>(state)];
		}
	}
}