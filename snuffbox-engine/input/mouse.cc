#include "mouse.h"
#include "buttons.h"

#include <cmath>

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Mouse::Mouse() :
			KeyQueue(MouseButtonsEnum::kNone + 1)
		{
			position_[0] = position_[1] = delta_[0] = delta_[1] = scroll_[0] = scroll_[1] = 0.0f;
		}

		//-----------------------------------------------------------------------------------------------
		void Mouse::MousePosition(int* px, int* py) const
		{
			if (px != nullptr)
			{
				*px = position_[0];
			}

			if (py != nullptr)
			{
				*py = position_[1];
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Mouse::MouseMovement(int* mx, int* my) const
		{
			if (mx != nullptr)
			{
				*mx = delta_[0];
			}

			if (my != nullptr)
			{
				*my = delta_[1];
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Mouse::MouseWheelDelta(int* sx, int* sy) const
		{
			if (sx != nullptr)
			{
				*sx = scroll_[0];
			}

			if (sy != nullptr)
			{
				*sy = scroll_[1];
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Mouse::SetPosition(int x, int y)
		{
			delta_[0] = position_[0];
			delta_[1] = position_[1];

			position_[0] = x;
			position_[1] = y;

			delta_[0] = x - delta_[0];
			delta_[1] = y - delta_[1];
		}

		//-----------------------------------------------------------------------------------------------
		void Mouse::SetScrollDelta(int x, int y)
		{
			scroll_[0] = x;
			scroll_[1] = y;
		}

		//-----------------------------------------------------------------------------------------------
		bool Mouse::Update()
		{
			unsigned int count = Flush();

			float adx = std::fabsf(static_cast<float>(delta_[0]));
			float ady = std::fabsf(static_cast<float>(delta_[1]));

			float sx = std::fabsf(static_cast<float>(scroll_[0]));
			float sy = std::fabsf(static_cast<float>(scroll_[1]));

			bool used = count > 0 || adx > 0.0f || ady > 0.0f || sx > 0.0f || sy > 0.0f;

			delta_[0] = delta_[1] = scroll_[0] = scroll_[1] = 0;

			return used;
		}
	}
}