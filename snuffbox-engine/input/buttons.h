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

			/**
			* @brief Convenience function to start the unrolling loop
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function to call each loop unroll
			*/
			static void MouseButtonsLoop(KeyCodes::LoopCallback to_call);
		};

        SNUFF_DECL_KEY_CODE(MouseButtons, Left,		GLFW_MOUSE_BUTTON_1);
        SNUFF_DECL_KEY_CODE(MouseButtons, Right,	GLFW_MOUSE_BUTTON_2);
        SNUFF_DECL_KEY_CODE(MouseButtons, Middle,	GLFW_MOUSE_BUTTON_3);
        SNUFF_DECL_KEY_CODE(MouseButtons, Button4,	GLFW_MOUSE_BUTTON_4);
        SNUFF_DECL_KEY_CODE(MouseButtons, Button5,	GLFW_MOUSE_BUTTON_5);
        SNUFF_DECL_KEY_CODE(MouseButtons, Button6,	GLFW_MOUSE_BUTTON_6);
        SNUFF_DECL_KEY_CODE(MouseButtons, Button7,	GLFW_MOUSE_BUTTON_7);
        SNUFF_DECL_KEY_CODE(MouseButtons, Button8,	GLFW_MOUSE_BUTTON_8);
        SNUFF_DECL_KEY_CODE(MouseButtons, Any,		GLFW_MOUSE_BUTTON_8 + 1);
        SNUFF_DECL_KEY_CODE(MouseButtons, None,		GLFW_MOUSE_BUTTON_8 + 2);

		/**
		* @struct snuffbox::engine::ControllerButtons
		* @brief A helper struct to enumerate all controller buttons and axes and assign them a string value
		* @remarks This follows the standard Xbox 360 controller layout
		* @author Daniel Konings
		*/
		struct ControllerButtons
		{
			SNUFF_BEGIN_KEY_CODES;

			/**
			* @brief The different available axes
			*/
			enum Axes : KeyCodes::KeyCode
			{
				kLeftStickX, //!< The left stick horizontal axis
				kLeftStickY, //!< The left stick vertical axis
				kRightStickX, //!< The right stick horizontal axis
				kRightStickY, //!< The right stick vertical axis
				kLT, //!< The left trigger axis
				kRT //!< The right trigger axis
			};

			/**
			* @brief Convenience function to start the unrolling loop
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function to call each loop unroll
			*/
			static void ControllerButtonsLoop(KeyCodes::LoopCallback to_call);
		};

        SNUFF_DECL_KEY_CODE(ControllerButtons, A,			0);
        SNUFF_DECL_KEY_CODE(ControllerButtons, B,			1);
        SNUFF_DECL_KEY_CODE(ControllerButtons, X,			2);
        SNUFF_DECL_KEY_CODE(ControllerButtons, Y,			3);
		SNUFF_DECL_KEY_CODE(ControllerButtons, LB,			4);
		SNUFF_DECL_KEY_CODE(ControllerButtons, RB,			5);
		SNUFF_DECL_KEY_CODE(ControllerButtons, Select,		6);
		SNUFF_DECL_KEY_CODE(ControllerButtons, Start,		7);
		SNUFF_DECL_KEY_CODE(ControllerButtons, LStick,		8);
		SNUFF_DECL_KEY_CODE(ControllerButtons, RStick,		9);
        SNUFF_DECL_KEY_CODE(ControllerButtons, Up,			10);
		SNUFF_DECL_KEY_CODE(ControllerButtons, Right,		11);
        SNUFF_DECL_KEY_CODE(ControllerButtons, Down,		12);
        SNUFF_DECL_KEY_CODE(ControllerButtons, Left,		13);
        SNUFF_DECL_KEY_CODE(ControllerButtons, Any,         15);
        SNUFF_DECL_KEY_CODE(ControllerButtons, None,		16);
	}
}
