#pragma once

#include "eastl.h"
#include "../js/js_defines.h"

#include "../services/window_service.h"

struct GLFWwindow;

namespace snuffbox
{
	namespace graphics
	{
		class Renderer;
	}

	namespace engine
	{
		class SnuffboxApp;
		class CVar;
		class Input;

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

			Window();

			/**
			* @brief Initialises the window with a width and height
			* @param[in] title (const snuffbox::engine::String&) The title of the window
			* @param[in] input (const snuffbox::engine::Input*) Used to check if the input callback from GLFW should be set
			* @param[in] width (unsigned int) The width of the window, default = snuffbox::engine::Window::DEFAULT_WIDTH_
			* @param[in] height (unsigned int) The height of the window, default = snuffbox::engine::Window::DEFAULT_HEIGHT_
			*/
			void Initialise(const String& title, const Input* input, unsigned int width = DEFAULT_WIDTH_, unsigned int height = DEFAULT_HEIGHT_);

			/**
			* @brief Creates the renderer
			* @param[in] width (unsigned int) The intial x resolution, in pixels, for the renderer
			* @param[in] height (unsigned int) The initial y resoluton, in pixels, for the renderer
			* @return (bool) Were we able to initialise the renderer?
			*/
			bool InitialiseRenderer(unsigned int width, unsigned int height);

			/**
			* @brief Creates the full 'title - SNUFF_WINDOW_TITLE' string
			* @param[in] title (const snuffbox::engine::String&) The application title
			* @return (snuffbox::engine::String) The created title
			*/
			static String CreateTitle(const String& title);

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

			/**
			* @brief Used to print out errors thrown by GLFW
			* @param[in] error (int) The error code
			* @param[in] description (const char*) The human-readable error
			*/
			static void GLFWErrorCallback(int error, const char* description);

			/**
			* @see snuffbox::graphics::Renderer::Status
			*/
			static void RendererErrorCallback(const char* msg, bool error);

		private:

			static const unsigned int DEFAULT_WIDTH_; //!< The default window width
			static const unsigned int DEFAULT_HEIGHT_; //!< The default window height

			String title_; //!< The title of the window
			unsigned int width_; //!< The width of the window
			unsigned int height_; //!< The height of the window
			bool should_close_; //!< Should the window be closed?

			GLFWwindow* window_; //!< The actual GLFW window
			Queue<WindowCommand> command_queue_; //!< The command queue to use with the window, to execute commands on the main thread

			UniquePtr<graphics::Renderer> renderer_; //!< The renderer owned by the window

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
