#pragma once

#include "../core/eastl.h"

#define SNUFF_DECL_KEY_CODE(key, val)				\
static const unsigned char k ## key = val;			\
template <>											\
struct Key<val>										\
{													\
	static const char* to_string() { return #key; } \
	static const bool is_valid = true;				\
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

			typedef void(*LoopCallback)(unsigned char, const char*);

			/**
			* @struct snuffbox::engine::KeyCodes::Key<unsigned char>
			* @brief Used to create a keycode definition along with a string value per keycode value
			* @author Daniel Konings
			*/
			template <unsigned char T>
			struct Key 
			{
				/**
				* @return (const char*) Any unused keycode will return 'unused' as its string value
				*/
				static const char* to_string()
				{
					return "unused";
				}

				static const bool is_valid = false; //!< Validity of this keycode
			};

			SNUFF_DECL_KEY_CODE(Any, 0x00);
			SNUFF_DECL_KEY_CODE(Backspace, 0x08);
			SNUFF_DECL_KEY_CODE(Tab, 0x09);
			SNUFF_DECL_KEY_CODE(Clear, 0x0C);
			SNUFF_DECL_KEY_CODE(Enter, 0x0D);
			SNUFF_DECL_KEY_CODE(Shift, 0x10);
			SNUFF_DECL_KEY_CODE(Control, 0x11);
			SNUFF_DECL_KEY_CODE(Alt, 0x12);
			SNUFF_DECL_KEY_CODE(Pause, 0x13);
			SNUFF_DECL_KEY_CODE(CapsLock, 0x14);
			SNUFF_DECL_KEY_CODE(Escape, 0x1B);
			SNUFF_DECL_KEY_CODE(Space, 0x20);
			SNUFF_DECL_KEY_CODE(PageUp, 0x21);
			SNUFF_DECL_KEY_CODE(PageDown, 0x22);
			SNUFF_DECL_KEY_CODE(End, 0x23);
			SNUFF_DECL_KEY_CODE(Home, 0x24);
			SNUFF_DECL_KEY_CODE(Left, 0x25);
			SNUFF_DECL_KEY_CODE(Up, 0x26);
			SNUFF_DECL_KEY_CODE(Right, 0x27);
			SNUFF_DECL_KEY_CODE(Down, 0x28);
			SNUFF_DECL_KEY_CODE(Select, 0x29);
			SNUFF_DECL_KEY_CODE(Print, 0x2A);
			SNUFF_DECL_KEY_CODE(Execute, 0x2B);
			SNUFF_DECL_KEY_CODE(PrintScreen, 0x2C);
			SNUFF_DECL_KEY_CODE(Insert, 0x2D);
			SNUFF_DECL_KEY_CODE(Delete, 0x2E);
			SNUFF_DECL_KEY_CODE(Help, 0x2F);
			SNUFF_DECL_KEY_CODE(Zero, 0x30);
			SNUFF_DECL_KEY_CODE(One, 0x31);
			SNUFF_DECL_KEY_CODE(Two, 0x32);
			SNUFF_DECL_KEY_CODE(Three, 0x33);
			SNUFF_DECL_KEY_CODE(Four, 0x34);
			SNUFF_DECL_KEY_CODE(Five, 0x35);
			SNUFF_DECL_KEY_CODE(Six, 0x36);
			SNUFF_DECL_KEY_CODE(Seven, 0x37);
			SNUFF_DECL_KEY_CODE(Eight, 0x38);
			SNUFF_DECL_KEY_CODE(Nine, 0x39);
			SNUFF_DECL_KEY_CODE(A, 0x41);
			SNUFF_DECL_KEY_CODE(B, 0x42);
			SNUFF_DECL_KEY_CODE(C, 0x43);
			SNUFF_DECL_KEY_CODE(D, 0x44);
			SNUFF_DECL_KEY_CODE(E, 0x45);
			SNUFF_DECL_KEY_CODE(F, 0x46);
			SNUFF_DECL_KEY_CODE(G, 0x47);
			SNUFF_DECL_KEY_CODE(H, 0x48);
			SNUFF_DECL_KEY_CODE(I, 0x49);
			SNUFF_DECL_KEY_CODE(J, 0x4A);
			SNUFF_DECL_KEY_CODE(K, 0x4B);
			SNUFF_DECL_KEY_CODE(L, 0x4C);
			SNUFF_DECL_KEY_CODE(M, 0x4D);
			SNUFF_DECL_KEY_CODE(N, 0x4E);
			SNUFF_DECL_KEY_CODE(O, 0x4F);
			SNUFF_DECL_KEY_CODE(P, 0x50);
			SNUFF_DECL_KEY_CODE(Q, 0x51);
			SNUFF_DECL_KEY_CODE(R, 0x52);
			SNUFF_DECL_KEY_CODE(S, 0x53);
			SNUFF_DECL_KEY_CODE(T, 0x54);
			SNUFF_DECL_KEY_CODE(U, 0x55);
			SNUFF_DECL_KEY_CODE(V, 0x56);
			SNUFF_DECL_KEY_CODE(W, 0x57);
			SNUFF_DECL_KEY_CODE(X, 0x58);
			SNUFF_DECL_KEY_CODE(Y, 0x59);
			SNUFF_DECL_KEY_CODE(Z, 0x5A);
			SNUFF_DECL_KEY_CODE(LeftWindowsKey, 0x5B);
			SNUFF_DECL_KEY_CODE(RightWindowsKey, 0x5C);
			SNUFF_DECL_KEY_CODE(ApplicationsKey, 0x5D);
			SNUFF_DECL_KEY_CODE(Sleep, 0x5F);
			SNUFF_DECL_KEY_CODE(NumPad0, 0x60);
			SNUFF_DECL_KEY_CODE(NumPad1, 0x61);
			SNUFF_DECL_KEY_CODE(NumPad2, 0x62);
			SNUFF_DECL_KEY_CODE(NumPad3, 0x63);
			SNUFF_DECL_KEY_CODE(NumPad4, 0x64);
			SNUFF_DECL_KEY_CODE(NumPad5, 0x65);
			SNUFF_DECL_KEY_CODE(NumPad6, 0x66);
			SNUFF_DECL_KEY_CODE(NumPad7, 0x67);
			SNUFF_DECL_KEY_CODE(NumPad8, 0x68);
			SNUFF_DECL_KEY_CODE(NumPad9, 0x69);
			SNUFF_DECL_KEY_CODE(Multiply, 0x6A);
			SNUFF_DECL_KEY_CODE(Plus, 0x6B);
			SNUFF_DECL_KEY_CODE(Seperator, 0x6C);
			SNUFF_DECL_KEY_CODE(Minus, 0x6D);
			SNUFF_DECL_KEY_CODE(Decimal, 0x6E);
			SNUFF_DECL_KEY_CODE(Divide, 0x6F);
			SNUFF_DECL_KEY_CODE(F1, 0x70);
			SNUFF_DECL_KEY_CODE(F2, 0x71);
			SNUFF_DECL_KEY_CODE(F3, 0x72);
			SNUFF_DECL_KEY_CODE(F4, 0x73);
			SNUFF_DECL_KEY_CODE(F5, 0x74);
			SNUFF_DECL_KEY_CODE(F6, 0x75);
			SNUFF_DECL_KEY_CODE(F7, 0x76);
			SNUFF_DECL_KEY_CODE(F8, 0x77);
			SNUFF_DECL_KEY_CODE(F9, 0x78);
			SNUFF_DECL_KEY_CODE(F10, 0x79);
			SNUFF_DECL_KEY_CODE(F11, 0x7A);
			SNUFF_DECL_KEY_CODE(F12, 0x7B);
			SNUFF_DECL_KEY_CODE(F13, 0x7C);
			SNUFF_DECL_KEY_CODE(F14, 0x7D);
			SNUFF_DECL_KEY_CODE(F15, 0x7E);
			SNUFF_DECL_KEY_CODE(F16, 0x7F);
			SNUFF_DECL_KEY_CODE(F17, 0x80);
			SNUFF_DECL_KEY_CODE(F18, 0x81);
			SNUFF_DECL_KEY_CODE(F19, 0x82);
			SNUFF_DECL_KEY_CODE(F20, 0x83);
			SNUFF_DECL_KEY_CODE(F21, 0x84);
			SNUFF_DECL_KEY_CODE(F22, 0x85);
			SNUFF_DECL_KEY_CODE(F23, 0x86);
			SNUFF_DECL_KEY_CODE(F24, 0x87);
			SNUFF_DECL_KEY_CODE(Numlock, 0x90);
			SNUFF_DECL_KEY_CODE(ScrollLock, 0x91);
			SNUFF_DECL_KEY_CODE(LeftShift, 0xA0);
			SNUFF_DECL_KEY_CODE(RightShift, 0xA1);
			SNUFF_DECL_KEY_CODE(LeftControl, 0xA2);
			SNUFF_DECL_KEY_CODE(RightControl, 0xA3);
			SNUFF_DECL_KEY_CODE(LeftMenu, 0xA4);
			SNUFF_DECL_KEY_CODE(RightMenu, 0xA5);
			SNUFF_DECL_KEY_CODE(BrowserBack, 0xA6);
			SNUFF_DECL_KEY_CODE(BrowserForward, 0xA7);
			SNUFF_DECL_KEY_CODE(BrowserRefresh, 0xA8);
			SNUFF_DECL_KEY_CODE(BrowserStop, 0xA9);
			SNUFF_DECL_KEY_CODE(BrowserSearch, 0xAA);
			SNUFF_DECL_KEY_CODE(BrowserFavorites, 0xAB);
			SNUFF_DECL_KEY_CODE(BrowserHome, 0xAC);
			SNUFF_DECL_KEY_CODE(VolumeMute, 0xAD);
			SNUFF_DECL_KEY_CODE(VolumeDown, 0xAE);
			SNUFF_DECL_KEY_CODE(VolumeUp, 0xAF);
			SNUFF_DECL_KEY_CODE(NextTrack, 0xB0);
			SNUFF_DECL_KEY_CODE(PreviousTrack, 0xB1);
			SNUFF_DECL_KEY_CODE(StopMedia, 0xB2);
			SNUFF_DECL_KEY_CODE(PlayPause, 0xB3);
			SNUFF_DECL_KEY_CODE(LaunchMail, 0xB4);
			SNUFF_DECL_KEY_CODE(SelectMedia, 0xB5);
			SNUFF_DECL_KEY_CODE(LaunchApp1, 0xB6);
			SNUFF_DECL_KEY_CODE(LaunchApp2, 0xB7);
			SNUFF_DECL_KEY_CODE(OEM1, 0xBA);
			SNUFF_DECL_KEY_CODE(OEMPlus, 0xB8);
			SNUFF_DECL_KEY_CODE(OEMComma, 0xBC);
			SNUFF_DECL_KEY_CODE(OEMMinus, 0xBD);
			SNUFF_DECL_KEY_CODE(OEMPeriod, 0xBE);
			SNUFF_DECL_KEY_CODE(OEM2, 0xBF);
			SNUFF_DECL_KEY_CODE(OEM3, 0xC0);
			SNUFF_DECL_KEY_CODE(OEM4, 0xDB);
			SNUFF_DECL_KEY_CODE(OEM5, 0xDC);
			SNUFF_DECL_KEY_CODE(OEM6, 0xDD);
			SNUFF_DECL_KEY_CODE(OEM7, 0xDE);
			SNUFF_DECL_KEY_CODE(OEM8, 0xDF);
			SNUFF_DECL_KEY_CODE(OEM102, 0xE2);
			SNUFF_DECL_KEY_CODE(Process, 0xE5);
			SNUFF_DECL_KEY_CODE(Packet, 0xE7);
			SNUFF_DECL_KEY_CODE(Attn, 0xF6);
			SNUFF_DECL_KEY_CODE(CrSel, 0xF7);
			SNUFF_DECL_KEY_CODE(ExSel, 0xF8);
			SNUFF_DECL_KEY_CODE(EraseEOF, 0xF9);
			SNUFF_DECL_KEY_CODE(Play, 0xFA);
			SNUFF_DECL_KEY_CODE(Zoom, 0xFB);
			SNUFF_DECL_KEY_CODE(PA1, 0xFD);
			SNUFF_DECL_KEY_CODE(OEMClear, 0xFE);
			SNUFF_DECL_KEY_CODE(None, 0xFF);

			/**
			* @brief Convenience function to start the unrolling loop
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function to call each loop unroll
			*/
			static void KeyCodeLoop(KeyCodes::LoopCallback to_call);
		};


		/**
		* @struct snuffbox::engine::KeyLoop<i, valid>
		* @brief Used to iterate over the declared keycodes and call a function per keycode/string combination
		* @author Daniel Konings
		*/
		template <unsigned char i, bool valid>
		struct KeyCodeLoop {};

		/**
		* @struct snuffbox::engine::KeyLoop<i, false>
		* @brief Invalid keycode iteration, it was never declared in snuffbox::engine::KeyCodes
		* @author Daniel Konings
		*/
		template <unsigned char i>
		struct KeyCodeLoop<i, false>
		{
			/**
			* @brief The unroll function, continues the loop
			* @param[in] void(*to_call)(unsigned char, const char*) The function pointer to call
			*/
			static void loop(void(*to_call)(unsigned char, const char*))
			{
				KeyCodeLoop<i + 1, KeyCodes::Key<i + 1>::is_valid>::loop(to_call);
			}
		};

		/**
		* @struct snuffbox::engine::KeyLoop<i, valid>
		* @brief Valid keycode iteration, it was declared in snuffbox::engine::KeyCodes
		* @author Daniel Konings
		*/
		template <unsigned char i>
		struct KeyCodeLoop<i, true>
		{
			/**
			* @brief The unroll function, does the a call for each keycode in the loop with the provided function pointer
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function pointer to call
			*/
			static void loop(KeyCodes::LoopCallback to_call)
			{
				to_call(i, KeyCodes::Key<i>::to_string());
				KeyCodeLoop<i + 1, KeyCodes::Key<i + 1>::is_valid>::loop(to_call);
			}
		};

		/**
		* @struct snuffbox::engine::KeyLoop<255, true>
		* @brief The final iteration of the key loop if the key is valid, this finalises the unroll
		* @author Daniel Konings
		*/
		template <>
		struct KeyCodeLoop<255, true>
		{
			/**
			* @brief Final unroll, does the final call with the provided function pointer
			* @param[in] to_call (snuffbox::engine::KeyCodes::LoopCallback) The function pointer to call
			*/
			static void loop(KeyCodes::LoopCallback to_call)
			{
				to_call(255, KeyCodes::Key<255>::to_string());
			}
		};

		/**
		* @struct snuffbox::engine::KeyLoop<255, false>
		* @brief The final iteration of the key loop if the key is invalid, this finalises the unroll
		* @author Daniel Konings
		*/
		template <>
		struct KeyCodeLoop<255, false>
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
