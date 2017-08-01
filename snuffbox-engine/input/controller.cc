#include "../services/log_service.h"
#include "controller.h"
#include "input.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		const float Controller::DEFAULT_DEAD_ZONE_ = 0.5f;

		//-----------------------------------------------------------------------------------------------
		Controller::Controller(unsigned int id) :
			KeyQueue(ControllerButtonsEnum::kNone + 1, ControllerButtonsEnum::kAny),
			id_(id),
			dead_zone_(DEFAULT_DEAD_ZONE_),
			connected_(false),
			axes_{ 0.0f, 0.0f, 0.0f, 0.0f }
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Controller::Poll()
		{
			if (glfwJoystickPresent(id_) == GLFW_TRUE && connected_ == false)
			{
				OnConnect();
			}
			else if (glfwJoystickPresent(id_) == GLFW_FALSE && connected_ == true)
			{
				OnDisconnect();
			}

			if (connected_ == false)
			{
				return;
			}

			ProcessButtons();
			ProcessAxes();

			Flush();
		}

		//-----------------------------------------------------------------------------------------------
		void Controller::ProcessButtons()
		{
			int count;
			const unsigned char* buttons = glfwGetJoystickButtons(id_, &count);

			if (last_state_.size() != count)
			{
				last_state_.resize(count);

				for (int i = 0; i < count; ++i)
				{
					last_state_.at(i) = GLFW_RELEASE;
				}
			}
			
			unsigned char current;
			for (int i = 0; i < count; ++i)
			{
				unsigned char& last = last_state_.at(i);
				current = buttons[i];

				if (last == GLFW_RELEASE && current == GLFW_PRESS)
				{
					PostEvent(Input::CreateKeyEvent(i, GLFW_PRESS));
				}
				else if (last == GLFW_PRESS && current == GLFW_RELEASE)
				{
					PostEvent(Input::CreateKeyEvent(i, GLFW_RELEASE));
				}

				last = current;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Controller::ProcessAxes()
		{
			int count;
			const float* axes = glfwGetJoystickAxes(id_, &count);

			if (axes_.size() != count)
			{
				axes_.resize(count);

				for (int i = 0; i < count; ++i)
				{
					axes_.at(i) = 0.0f;
				}
			}

			for (int i = 0; i < count; ++i)
			{
				axes_.at(i) = DeadZone(axes[i]);
			}
		}

		//-----------------------------------------------------------------------------------------------
		float Controller::DeadZone(float val) const
		{
			float sign = val == 0.0f ? 0.0f : val / std::fabsf(val);
			float abs_val = std::fabsf(val);

			abs_val = std::fmaxf(0.0f, abs_val - dead_zone_);
			float axis = abs_val / (1.0f - dead_zone_);

			return std::fminf(1.0f, std::fmaxf(0.0f, axis)) * sign;
		}

		//-----------------------------------------------------------------------------------------------
		float Controller::Axis(ControllerButtons::Axes axis) const
		{
			if (axis >= axes_.size())
			{
				return 0.0f;
			}

			return axes_.at(axis);
		}

		//-----------------------------------------------------------------------------------------------
		void Controller::OnConnect()
		{
			Services::Get<LogService>().Log(console::LogSeverity::kInfo, "Controller ({0}) connected: {1}", id_, glfwGetJoystickName(id_));
			connected_ = true;
		}

		//-----------------------------------------------------------------------------------------------
		void Controller::OnDisconnect()
		{
			Services::Get<LogService>().Log(console::LogSeverity::kInfo, "Controller ({0}) disconnected", id_);
			connected_ = false;
		}

		//-----------------------------------------------------------------------------------------------
		void Controller::set_dead_zone(float dz)
		{
			dead_zone_ = dz;
		}

		//-----------------------------------------------------------------------------------------------
		bool Controller::connected() const
		{
			return connected_;
		}
	}
}