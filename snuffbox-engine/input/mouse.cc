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
			position_[0] = position_[1] = delta_[0] = delta_[1] = 0;
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
		bool Mouse::Update()
		{
			unsigned int count = Flush();
			bool used = count > 0 || std::fabsf(static_cast<float>(delta_[0])) > 0.0f || std::fabsf(static_cast<float>(delta_[1]));

			delta_[0] = delta_[1] = 0;

			return used;
		}
	}
}