#pragma once

#include "keycodes.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @struct snuffbox::engine::MouseButtongs
		* @brief A helper struct to enumerate all mouse button codes and assign them a string value
		* @author Daniel Konings
		*/
		struct MouseButtons
		{
			SNUFF_BEGIN_KEY_CODES;

			SNUFF_DECL_KEY_CODE(Left,		GLFW_MOUSE_BUTTON_1);
			SNUFF_DECL_KEY_CODE(Right,		GLFW_MOUSE_BUTTON_2);
			SNUFF_DECL_KEY_CODE(Middle,		GLFW_MOUSE_BUTTON_3);
			SNUFF_DECL_KEY_CODE(Button4,	GLFW_MOUSE_BUTTON_4);
			SNUFF_DECL_KEY_CODE(Button5,	GLFW_MOUSE_BUTTON_5);
			SNUFF_DECL_KEY_CODE(Button6,	GLFW_MOUSE_BUTTON_6);
			SNUFF_DECL_KEY_CODE(Button7,	GLFW_MOUSE_BUTTON_7);
			SNUFF_DECL_KEY_CODE(Button8,	GLFW_MOUSE_BUTTON_8);
			SNUFF_DECL_KEY_CODE(Any,		GLFW_MOUSE_BUTTON_8 + 1);
			SNUFF_DECL_KEY_CODE(None,		GLFW_MOUSE_BUTTON_8 + 2);

			/**
			* @brief Convenience function to start the unrolling loop
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function to call each loop unroll
			*/
			static void MouseButtonsLoop(KeyCodes::LoopCallback to_call);
		};

		/**
		* @struct snuffbox::engine::ControllerButtons
		* @brief A helper struct to enumerate all controller buttons and axes and assign them a string value
		* @remarks This follows the standard Xbox 360 controller layout
		* @author Daniel Konings
		*/
		struct ControllerButtons
		{
			SNUFF_BEGIN_KEY_CODES;

			SNUFF_DECL_KEY_CODE(A,			GLFW_JOYSTICK_1);
			SNUFF_DECL_KEY_CODE(B,			GLFW_JOYSTICK_2);
			SNUFF_DECL_KEY_CODE(X,			GLFW_JOYSTICK_3);
			SNUFF_DECL_KEY_CODE(Y,			GLFW_JOYSTICK_4);
			SNUFF_DECL_KEY_CODE(Up,			GLFW_JOYSTICK_5);
			SNUFF_DECL_KEY_CODE(Down,		GLFW_JOYSTICK_6);
			SNUFF_DECL_KEY_CODE(Left,		GLFW_JOYSTICK_7);
			SNUFF_DECL_KEY_CODE(Right,		GLFW_JOYSTICK_8);
			SNUFF_DECL_KEY_CODE(Start,		GLFW_JOYSTICK_9);
			SNUFF_DECL_KEY_CODE(Select,		GLFW_JOYSTICK_10);
			SNUFF_DECL_KEY_CODE(LStick,		GLFW_JOYSTICK_11);
			SNUFF_DECL_KEY_CODE(RStick,		GLFW_JOYSTICK_12);
			SNUFF_DECL_KEY_CODE(RB,			GLFW_JOYSTICK_13);
			SNUFF_DECL_KEY_CODE(LB,			GLFW_JOYSTICK_14);
			SNUFF_DECL_KEY_CODE(Guide,		GLFW_JOYSTICK_15);
			SNUFF_DECL_KEY_CODE(Any,		GLFW_JOYSTICK_16);
			SNUFF_DECL_KEY_CODE(None,		GLFW_JOYSTICK_16 + 1);

			/**
			* @brief The different available axes
			*/
			enum Axes : KeyCodes::KeyCode
			{
				kLeftAxis, //!< The left stick axis
				kRightAxis, //!< The right stick axis
				kLT, //!< The left trigger axis
				kRT //!< The right trigger axis
			};

			/**
			* @brief Convenience function to start the unrolling loop
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function to call each loop unroll
			*/
			static void ControllerButtonsLoop(KeyCodes::LoopCallback to_call);
		};
	}
}