#pragma once

#include "../core/eastl.h"
#include <GLFW/glfw3.h>

#include <functional>
#include <typeinfo>

#define SNUFF_BEGIN_KEY_CODES							\
template <KeyCodes::KeyCode T>							\
struct Key												\
{														\
	static const char* to_string(){ return "unused"; }	\
	static const bool is_valid = false;					\
}

#define SNUFF_DECL_KEY_CODE(type, key, val)                 \
namespace type ## Enum                                      \
{                                                           \
    static const KeyCodes::KeyCode k ## key = val;          \
}                                                           \
template <>                                                 \
struct type::Key<val>                                       \
{                                                           \
    static const char* to_string() { return #key; }         \
    static const bool is_valid = true;                      \
}

namespace snuffbox
{
	namespace engine
	{
		/**
		* @struct snuffbox::engine::KeyCodes
		* @brief A helper struct to enumerate all keycodes and assign them a string value
		* @author Daniel Konings
		*/
		struct KeyCodes
		{
			typedef unsigned short KeyCode;
			typedef std::function<void(KeyCode, const char*)> LoopCallback;

            SNUFF_BEGIN_KEY_CODES;

			/**
			* @brief Convenience function to start the unrolling loop
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function to call each loop unroll
			*/
            static void KeyCodeLoop(KeyCodes::LoopCallback to_call);
        };

        SNUFF_DECL_KEY_CODE(KeyCodes, Any,				GLFW_KEY_SPACE - 1);
        SNUFF_DECL_KEY_CODE(KeyCodes, Space,			GLFW_KEY_SPACE);
        SNUFF_DECL_KEY_CODE(KeyCodes, Apostrophe,		GLFW_KEY_APOSTROPHE);
        SNUFF_DECL_KEY_CODE(KeyCodes, Comma,			GLFW_KEY_COMMA);
        SNUFF_DECL_KEY_CODE(KeyCodes, Minus,			GLFW_KEY_MINUS);
        SNUFF_DECL_KEY_CODE(KeyCodes, Period,			GLFW_KEY_PERIOD);
        SNUFF_DECL_KEY_CODE(KeyCodes, Slash,			GLFW_KEY_SLASH);
        SNUFF_DECL_KEY_CODE(KeyCodes, 0,				GLFW_KEY_0);
        SNUFF_DECL_KEY_CODE(KeyCodes, 1,				GLFW_KEY_1);
        SNUFF_DECL_KEY_CODE(KeyCodes, 2,				GLFW_KEY_2);
        SNUFF_DECL_KEY_CODE(KeyCodes, 3,				GLFW_KEY_3);
        SNUFF_DECL_KEY_CODE(KeyCodes, 4,				GLFW_KEY_4);
        SNUFF_DECL_KEY_CODE(KeyCodes, 5,				GLFW_KEY_5);
        SNUFF_DECL_KEY_CODE(KeyCodes, 6,				GLFW_KEY_6);
        SNUFF_DECL_KEY_CODE(KeyCodes, 7,				GLFW_KEY_7);
        SNUFF_DECL_KEY_CODE(KeyCodes, 8,				GLFW_KEY_8);
        SNUFF_DECL_KEY_CODE(KeyCodes, 9,				GLFW_KEY_9);
        SNUFF_DECL_KEY_CODE(KeyCodes, Semicolon,		GLFW_KEY_SEMICOLON);
        SNUFF_DECL_KEY_CODE(KeyCodes, Equal,			GLFW_KEY_EQUAL);
        SNUFF_DECL_KEY_CODE(KeyCodes, A,				GLFW_KEY_A);
        SNUFF_DECL_KEY_CODE(KeyCodes, B,				GLFW_KEY_B);
        SNUFF_DECL_KEY_CODE(KeyCodes, C,				GLFW_KEY_C);
        SNUFF_DECL_KEY_CODE(KeyCodes, D,				GLFW_KEY_D);
        SNUFF_DECL_KEY_CODE(KeyCodes, E,				GLFW_KEY_E);
        SNUFF_DECL_KEY_CODE(KeyCodes, F,				GLFW_KEY_F);
        SNUFF_DECL_KEY_CODE(KeyCodes, G,				GLFW_KEY_G);
        SNUFF_DECL_KEY_CODE(KeyCodes, H,				GLFW_KEY_H);
        SNUFF_DECL_KEY_CODE(KeyCodes, I,				GLFW_KEY_I);
        SNUFF_DECL_KEY_CODE(KeyCodes, J,				GLFW_KEY_J);
        SNUFF_DECL_KEY_CODE(KeyCodes, K,				GLFW_KEY_K);
        SNUFF_DECL_KEY_CODE(KeyCodes, L,				GLFW_KEY_L);
        SNUFF_DECL_KEY_CODE(KeyCodes, M,				GLFW_KEY_M);
        SNUFF_DECL_KEY_CODE(KeyCodes, N,				GLFW_KEY_N);
        SNUFF_DECL_KEY_CODE(KeyCodes, O,				GLFW_KEY_O);
        SNUFF_DECL_KEY_CODE(KeyCodes, P,				GLFW_KEY_P);
        SNUFF_DECL_KEY_CODE(KeyCodes, Q,				GLFW_KEY_Q);
        SNUFF_DECL_KEY_CODE(KeyCodes, R,				GLFW_KEY_R);
        SNUFF_DECL_KEY_CODE(KeyCodes, S,				GLFW_KEY_S);
        SNUFF_DECL_KEY_CODE(KeyCodes, T,				GLFW_KEY_T);
        SNUFF_DECL_KEY_CODE(KeyCodes, U,				GLFW_KEY_U);
        SNUFF_DECL_KEY_CODE(KeyCodes, V,				GLFW_KEY_V);
        SNUFF_DECL_KEY_CODE(KeyCodes, W,				GLFW_KEY_W);
        SNUFF_DECL_KEY_CODE(KeyCodes, X,				GLFW_KEY_X);
        SNUFF_DECL_KEY_CODE(KeyCodes, Y,				GLFW_KEY_Y);
        SNUFF_DECL_KEY_CODE(KeyCodes, Z,				GLFW_KEY_Z);
        SNUFF_DECL_KEY_CODE(KeyCodes, LeftBracket,		GLFW_KEY_LEFT_BRACKET);
        SNUFF_DECL_KEY_CODE(KeyCodes, Backslash,        GLFW_KEY_BACKSLASH);
        SNUFF_DECL_KEY_CODE(KeyCodes, RightBracket,		GLFW_KEY_RIGHT_BRACKET);
        SNUFF_DECL_KEY_CODE(KeyCodes, GraveAccent,		GLFW_KEY_GRAVE_ACCENT);
        SNUFF_DECL_KEY_CODE(KeyCodes, World1,			GLFW_KEY_WORLD_1);
        SNUFF_DECL_KEY_CODE(KeyCodes, World2,			GLFW_KEY_WORLD_2);
        SNUFF_DECL_KEY_CODE(KeyCodes, Escape,			GLFW_KEY_ESCAPE);
        SNUFF_DECL_KEY_CODE(KeyCodes, Enter,			GLFW_KEY_ENTER);
        SNUFF_DECL_KEY_CODE(KeyCodes, Tab,				GLFW_KEY_TAB);
        SNUFF_DECL_KEY_CODE(KeyCodes, Backspace,		GLFW_KEY_BACKSPACE);
        SNUFF_DECL_KEY_CODE(KeyCodes, Insert,			GLFW_KEY_INSERT);
        SNUFF_DECL_KEY_CODE(KeyCodes, Delete,			GLFW_KEY_DELETE);
        SNUFF_DECL_KEY_CODE(KeyCodes, Right,			GLFW_KEY_RIGHT);
        SNUFF_DECL_KEY_CODE(KeyCodes, Down,				GLFW_KEY_DOWN);
        SNUFF_DECL_KEY_CODE(KeyCodes, Left,				GLFW_KEY_LEFT);
        SNUFF_DECL_KEY_CODE(KeyCodes, Up,				GLFW_KEY_UP);
        SNUFF_DECL_KEY_CODE(KeyCodes, PageUp,			GLFW_KEY_PAGE_UP);
        SNUFF_DECL_KEY_CODE(KeyCodes, PageDown,			GLFW_KEY_PAGE_DOWN);
        SNUFF_DECL_KEY_CODE(KeyCodes, Home,				GLFW_KEY_HOME);
        SNUFF_DECL_KEY_CODE(KeyCodes, End,				GLFW_KEY_END);
        SNUFF_DECL_KEY_CODE(KeyCodes, CapsLock,			GLFW_KEY_CAPS_LOCK);
        SNUFF_DECL_KEY_CODE(KeyCodes, ScrollLock,		GLFW_KEY_SCROLL_LOCK);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumLock,			GLFW_KEY_NUM_LOCK);
        SNUFF_DECL_KEY_CODE(KeyCodes, Pause,			GLFW_KEY_PAUSE);
        SNUFF_DECL_KEY_CODE(KeyCodes, F1,				GLFW_KEY_F1);
        SNUFF_DECL_KEY_CODE(KeyCodes, F2,				GLFW_KEY_F2);
        SNUFF_DECL_KEY_CODE(KeyCodes, F3,				GLFW_KEY_F3);
        SNUFF_DECL_KEY_CODE(KeyCodes, F4,				GLFW_KEY_F4);
        SNUFF_DECL_KEY_CODE(KeyCodes, F5,				GLFW_KEY_F5);
        SNUFF_DECL_KEY_CODE(KeyCodes, F6,				GLFW_KEY_F6);
        SNUFF_DECL_KEY_CODE(KeyCodes, F7,				GLFW_KEY_F7);
        SNUFF_DECL_KEY_CODE(KeyCodes, F8,				GLFW_KEY_F8);
        SNUFF_DECL_KEY_CODE(KeyCodes, F9,				GLFW_KEY_F9);
        SNUFF_DECL_KEY_CODE(KeyCodes, F10,				GLFW_KEY_F10);
        SNUFF_DECL_KEY_CODE(KeyCodes, F11,				GLFW_KEY_F11);
        SNUFF_DECL_KEY_CODE(KeyCodes, F12,				GLFW_KEY_F12);
        SNUFF_DECL_KEY_CODE(KeyCodes, F13,				GLFW_KEY_F13);
        SNUFF_DECL_KEY_CODE(KeyCodes, F14,				GLFW_KEY_F14);
        SNUFF_DECL_KEY_CODE(KeyCodes, F15,				GLFW_KEY_F15);
        SNUFF_DECL_KEY_CODE(KeyCodes, F16,				GLFW_KEY_F16);
        SNUFF_DECL_KEY_CODE(KeyCodes, F17,				GLFW_KEY_F17);
        SNUFF_DECL_KEY_CODE(KeyCodes, F18,				GLFW_KEY_F18);
        SNUFF_DECL_KEY_CODE(KeyCodes, F19,				GLFW_KEY_F19);
        SNUFF_DECL_KEY_CODE(KeyCodes, F20,				GLFW_KEY_F20);
        SNUFF_DECL_KEY_CODE(KeyCodes, F21,				GLFW_KEY_F21);
        SNUFF_DECL_KEY_CODE(KeyCodes, F22,				GLFW_KEY_F22);
        SNUFF_DECL_KEY_CODE(KeyCodes, F23,				GLFW_KEY_F23);
        SNUFF_DECL_KEY_CODE(KeyCodes, F24,				GLFW_KEY_F24);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad0,			GLFW_KEY_KP_0);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad1,			GLFW_KEY_KP_1);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad2,			GLFW_KEY_KP_2);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad3,			GLFW_KEY_KP_3);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad4,			GLFW_KEY_KP_4);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad5,			GLFW_KEY_KP_5);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad6,			GLFW_KEY_KP_6);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad7,			GLFW_KEY_KP_7);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad8,			GLFW_KEY_KP_8);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPad9,			GLFW_KEY_KP_9);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPadDecimal,	GLFW_KEY_KP_DECIMAL);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPadDivide,		GLFW_KEY_KP_DIVIDE);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPadMultiply,	GLFW_KEY_KP_MULTIPLY);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPadSubtract,	GLFW_KEY_KP_SUBTRACT);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPadAdd,		GLFW_KEY_KP_ADD);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPadEnter,		GLFW_KEY_KP_ENTER);
        SNUFF_DECL_KEY_CODE(KeyCodes, NumPadEqual,		GLFW_KEY_KP_EQUAL);
        SNUFF_DECL_KEY_CODE(KeyCodes, LeftShift,		GLFW_KEY_LEFT_SHIFT);
        SNUFF_DECL_KEY_CODE(KeyCodes, LeftAlt,			GLFW_KEY_LEFT_ALT);
        SNUFF_DECL_KEY_CODE(KeyCodes, LeftControl,		GLFW_KEY_LEFT_CONTROL);
        SNUFF_DECL_KEY_CODE(KeyCodes, LeftSuper,		GLFW_KEY_LEFT_SUPER);
        SNUFF_DECL_KEY_CODE(KeyCodes, RightShift,		GLFW_KEY_RIGHT_SHIFT);
        SNUFF_DECL_KEY_CODE(KeyCodes, RightAlt,			GLFW_KEY_RIGHT_ALT);
        SNUFF_DECL_KEY_CODE(KeyCodes, RightControl,		GLFW_KEY_RIGHT_CONTROL);
        SNUFF_DECL_KEY_CODE(KeyCodes, RightSuper,		GLFW_KEY_RIGHT_SUPER);
        SNUFF_DECL_KEY_CODE(KeyCodes, Menu,				GLFW_KEY_MENU);
        SNUFF_DECL_KEY_CODE(KeyCodes, None,				GLFW_KEY_MENU + 1);

		/**
		* @struct snuffbox::engine::KeyLoop<T, i, valid, max>
		* @brief Used to iterate over the declared keycodes and call a function per keycode/string combination
		* @author Daniel Konings
		*/
        template <typename T, KeyCodes::KeyCode i, bool valid, KeyCodes::KeyCode max>
        struct KeyCodeLoop {};

		/**
		* @struct snuffbox::engine::KeyLoop<T, i, false, max>
		* @brief Invalid keycode iteration, it was never declared in snuffbox::engine::KeyCodes
		* @author Daniel Konings
		*/
        template <typename T, KeyCodes::KeyCode i, KeyCodes::KeyCode max>
        struct KeyCodeLoop<T, i, false, max>
		{
			/**
			* @brief The unroll function, continues the loop
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function pointer to call
			*/
            static void loop(KeyCodes::LoopCallback to_call)
            {
                const KeyCodes::KeyCode next = static_cast<KeyCodes::KeyCode>(i + 1);
                KeyCodeLoop<T, next, T::template Key<next>::is_valid, max>::loop(to_call);
			}
		};

		/**
		* @struct snuffbox::engine::KeyLoop<T, i, true, max>
		* @brief Valid keycode iteration, it was declared in snuffbox::engine::KeyCodes
		* @author Daniel Konings
		*/
        template <typename T, KeyCodes::KeyCode i, KeyCodes::KeyCode max>
        struct KeyCodeLoop<T, i, true, max>
		{
			/**
			* @brief The unroll function, does the a call for each keycode in the loop with the provided function pointer
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function pointer to call
			*/
            static void loop(KeyCodes::LoopCallback to_call)
			{
                const KeyCodes::KeyCode next = static_cast<KeyCodes::KeyCode>(i + 1);
                to_call(i, T::template Key<i>::to_string());
                KeyCodeLoop<T, next, T::template Key<next>::is_valid, max>::loop(to_call);
			}
		};

		/**
		* @struct snuffbox::engine::KeyLoop<T, max, true, max>
		* @brief The final iteration of the key loop if the key is valid, this finalises the unroll
		* @author Daniel Konings
		*/
        template <typename T, KeyCodes::KeyCode max>
        struct KeyCodeLoop<T, max, true, max>
		{
			/**
			* @brief Final unroll, does the final call with the provided function pointer
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function pointer to call
			*/
            static void loop(KeyCodes::LoopCallback to_call)
			{
                to_call(max, T::template Key<max>::to_string());
			}
		};

		/**
		* @struct snuffbox::engine::KeyLoop<T, max, false, max>
		* @brief The final iteration of the key loop if the key is invalid, this finalises the unroll
		* @author Daniel Konings
		*/
        template <typename T, KeyCodes::KeyCode max>
		struct KeyCodeLoop<T, max, false, max>
		{
			/**
			* @brief Final unroll, do nothing
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function pointer to call
			*/
            static void loop(KeyCodes::LoopCallback to_call)
			{

			}
		};
	}
}
