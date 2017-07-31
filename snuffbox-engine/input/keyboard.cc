#include "keyboard.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Keyboard::Keyboard()
		{
			for (KeyCodes::KeyCode i = 0; i < KEY_LENGTH_; ++i)
			{
				states_[i] = KeyState::kUp;
			}

			for (int i = 0; i < static_cast<int>(KeyState::kCount); ++i)
			{
                last_[i] = KeyCodesEnum::kNone;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Keyboard::PostEvent(const Keyboard::Event& evt)
		{
			queue_.push(evt);
		}

		//-----------------------------------------------------------------------------------------------
		void Keyboard::OnPressed(KeyCodes::KeyCode key)
		{
			states_[key] = KeyState::kPressed;
		}

		//-----------------------------------------------------------------------------------------------
		void Keyboard::OnReleased(KeyCodes::KeyCode key)
		{
			states_[key] = KeyState::kReleased;
		}

		//-----------------------------------------------------------------------------------------------
		void Keyboard::ResetStates()
		{
			for (KeyCodes::KeyCode i = 0; i < KEY_LENGTH_; ++i)
			{
				states_[i] = (states_[i] == KeyState::kPressed || states_[i] == KeyState::kDown) ? KeyState::kDown : KeyState::kUp;
			}
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int Keyboard::Flush()
		{
			unsigned int count = 0;

			ResetStates();

			KeyCodes::KeyCode key;
			KeyState state;

			while (queue_.empty() == false)
			{
				const Keyboard::Event& front = queue_.front();
				key = front.key;

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
	}
}
