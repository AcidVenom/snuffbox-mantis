#pragma once

#include "eastl.h"
#include "../js/js_defines.h"

#include "../services/window_service.h"

struct GLFWwindow;

namespace snuffbox
{
	namespace engine
	{
		class SnuffboxApp;
		class CVar;

		/**
		* @class snuffbox::engine::Window : [JSObject]
		* @brief Used as a wrapper around GLFW to create a window
		* @author Daniel Konings
		*/
		class Window : JS_OBJECT_MULTI WindowService
		{

			friend class SnuffboxApp;
			friend class Allocator;

		protected:

			/**
			* @struct snuffbox::engine::WindowCommand
			* @brief Used to execute commands on the main thread only
			* @remarks This is especially useful on Win32, as the Win32 API doesn't allow for function calls from another thread
			* @author Daniel Konings
			*/
			struct WindowCommand
			{
				/**
				* @brief The different commands
				*/
				enum Flags
				{
					kSetTitle, //!< Set the title of the window
					kSetSize //!< Set the size of the window
				};

				Flags flag; //!< Which command are we running?
				String title; //!< If we have a kSetTitle flag, this is the value to set
				unsigned int size[2]; //!< If we have a kSetSize flag, this is the value to set
			};

			/**
			* @brief Default constructor
			*/
			Window();

			/**
			* @brief Initialises the window with a width and height
			* @param[in] title (const snuffbox::engine::String&) The title of the window
			* @param[in] width (unsigned int) The width of the window, default = snuffbox::engine::Window::DEFAULT_WIDTH_
			* @param[in] height (unsigned int) The height of the window, default = snuffbox::engine::Window::DEFAULT_HEIGHT_
			*/
			void Initialise(const String& title, unsigned int width = DEFAULT_WIDTH_, unsigned int height = DEFAULT_HEIGHT_);

			/**
			* @see snuffbox::engine::WindowService::SetTitle
			*/
			void SetTitle(const String& title) override;

			/**
			* @see snuffbox::engine::WindowService::SetSize
			*/
			void SetSize(unsigned int width, unsigned int height) override;

			/**
			* @see snuffbox::engine::WindowService::Close
			*/
			void Close() override;

			/**
			* @see snuffbox::engine::WindowService::Title
			*/
			String Title() const override;

			/**
			* @see snuffbox::engine::WindowService::Width
			*/
			unsigned int Width() const override;

			/**
			* @see snuffbox::engine::WindowService::Height
			*/
			unsigned int Height() const override;

			/**
			* @see snuffbox::engine::WindowService::Closed
			*/
			bool Closed() const override;

			/**
			* @brief Poll events sent to the window from the OS
			*/
			void Poll();

			/**
			* @brief Shuts the window down
			*/
			void Shutdown();

		private:

			/**
			* @brief Used to print out errors thrown by GLFW
			* @param[in] error (int) The error code
			* @param[in] description (const char*) The human-readable error
			*/
			static void ErrorCallback(int error, const char* description);

			static const unsigned int DEFAULT_WIDTH_; //!< The default window width
			static const unsigned int DEFAULT_HEIGHT_; //!< The default window height

			String title_; //!< The title of the window
			unsigned int width_; //!< The width of the window
			unsigned int height_; //!< The height of the window
			bool should_close_; //!< Should the window be closed?

			GLFWwindow* window_; //!< The actual GLFW window
			Queue<WindowCommand> command_queue_; //!< The command queue to use with the window, to execute commands on the main thread
		public:

			JS_NAME_SINGLE(Window);
			JS_FUNCTION_DECL(close);
			JS_FUNCTION_DECL(setTitle);
			JS_FUNCTION_DECL(setSize);
			JS_FUNCTION_DECL(title);
			JS_FUNCTION_DECL(size);
		};
	}
}
