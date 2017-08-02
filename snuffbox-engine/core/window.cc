#include "window.h"

#include "../services/log_service.h"
#include "../input/input.h"

#include <snuffbox-graphics/platform/platform_renderer.h>
#include <GLFW/glfw3.h>

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		const unsigned int Window::DEFAULT_WIDTH_ = 1280;
		const unsigned int Window::DEFAULT_HEIGHT_ = 720;

		//-----------------------------------------------------------------------------------------------
		Window::Window() :
			title_(""),
			width_(0),
			height_(0),
			window_(nullptr),
			should_close_(false)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void Window::Initialise(const String& title, const Input* input, unsigned int width, unsigned int height)
		{
			LogService& log = Services::Get<LogService>();

			log.Assert(glfwInit() == GL_TRUE, "Could not initialise GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);

#if defined(SNUFF_USE_OGL) || defined(SNUFF_USE_VULKAN)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			log.Assert((window_ = glfwCreateWindow(width, height, CreateTitle(title).c_str(), nullptr, nullptr)) != nullptr,
						"GLFW window creation failed");

			title_ = title;
			width_ = width;
			height_ = height;

			if (input != nullptr)
			{
				glfwSetKeyCallback(window_, Input::KeyboardCallback);
				glfwSetMouseButtonCallback(window_, Input::MouseButtonCallback);
				glfwSetCursorPosCallback(window_, Input::MousePositionCallback);
				glfwSetScrollCallback(window_, Input::MouseScrollCallback);
			}

			log.Assert(InitialiseRenderer(width, height) == true, "Could not initialise the renderer");
		}

		//-----------------------------------------------------------------------------------------------
		bool Window::InitialiseRenderer(unsigned int width, unsigned int height)
		{
			LogService& log = Services::Get<LogService>();
			renderer_ = Memory::ConstructUnique<graphics::Renderer>(RendererErrorCallback);
			bool success;

#ifdef SNUFF_USE_VULKAN
			unsigned int ext_count = 0;
			const char** extensions;

			extensions = glfwGetRequiredInstanceExtensions(&ext_count);

			success = renderer_->Initialise(ext_count, extensions, width, height);
#else
			success = renderer_->Initialise(width, height);
#endif
			log.Log(console::LogSeverity::kSuccess, "Successfully initialised the renderer");

			return success;
		}

		//-----------------------------------------------------------------------------------------------
		String Window::CreateTitle(const String& title)
		{
			return title + " - " + SNUFF_WINDOW_TITLE;
		}

		//-----------------------------------------------------------------------------------------------
		void Window::SetTitle(const String& title)
		{
			LogService& log = Services::Get<LogService>();
			log.Assert(window_ != nullptr, "Attempted to change the title of a non-existing window");

			WindowCommand cmd;
			cmd.flag = WindowCommand::Flags::kSetTitle;
			cmd.title = title;

			command_queue_.push(cmd);
		}

		//-----------------------------------------------------------------------------------------------
		void Window::SetSize(unsigned int width, unsigned int height)
		{
			LogService& log = Services::Get<LogService>();
			log.Assert(window_ != nullptr, "Attempted to size a non-existing window");

			if (width == 0 || height == 0)
			{
				log.Log(console::LogSeverity::kError,
												"Cannot size the window to either a width or height of 0. Width: {0}, height: {1}",
												width,
												height);
				return;
			}

			WindowCommand cmd;
			cmd.flag = WindowCommand::Flags::kSetSize;
			cmd.size[0] = width;
			cmd.size[1] = height;

			command_queue_.push(cmd);
		}

		//-----------------------------------------------------------------------------------------------
		void Window::Close()
		{
			should_close_ = true;
			Services::Get<LogService>().Log(console::LogSeverity::kInfo, "Closing the window");
		}

		//-----------------------------------------------------------------------------------------------
		String Window::Title() const
		{
			return title_;
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int Window::Width() const
		{
			return width_;
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int Window::Height() const
		{
			return height_;
		}

		//-----------------------------------------------------------------------------------------------
		bool Window::Closed() const
		{
			return should_close_ == true || glfwWindowShouldClose(window_) == GL_TRUE;
		}

		//-----------------------------------------------------------------------------------------------
		void Window::Poll()
		{
			LogService& log = Services::Get<LogService>();

			auto SizeCommand = [=](const WindowCommand& cmd, LogService& l)
			{
				unsigned int width = cmd.size[0];
				unsigned int height = cmd.size[1];

				glfwSetWindowSize(window_, width, height);

				width_ = width;
				height_ = height;

				l.Log(console::LogSeverity::kInfo, "Resized the window to: {0}x{1}", width, height);
			};

			auto TitleCommand = [=](const WindowCommand& cmd, LogService& l)
			{
				const String& title = cmd.title;
				glfwSetWindowTitle(window_, CreateTitle(title).c_str());
				title_ = title;

				l.Log(console::LogSeverity::kInfo, "Renamed the window title to: {0}", title_);
			};

			while (command_queue_.empty() == false)
			{
				const WindowCommand& top = command_queue_.front();

				switch (top.flag)
				{
				case WindowCommand::Flags::kSetTitle:
					TitleCommand(top, log);
					break;

				case WindowCommand::Flags::kSetSize:
					SizeCommand(top, log);
					break;

				default:
					break;
				}

				command_queue_.pop();
			}

			glfwPollEvents();
		}

		//-----------------------------------------------------------------------------------------------
		void Window::Shutdown()
		{
			Services::Get<LogService>().Assert(window_ != nullptr, "Attempted to shut down the window, but it was never initialised");

			renderer_->Shutdown();

			glfwDestroyWindow(window_);
			glfwTerminate();
		}

		//-----------------------------------------------------------------------------------------------
		void Window::GLFWErrorCallback(int error, const char* description)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kError, "GLFW error: {0} -> {1}", error, description);
		}

		//-----------------------------------------------------------------------------------------------
		void Window::RendererErrorCallback(const char* msg, bool error)
		{
			Services::Get<LogService>().Log(error == true ? console::LogSeverity::kError : console::LogSeverity::kInfo, 
				error == true ? "Renderer error: {0}" : "{0}", msg);
		}

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_SINGLE(Window, JS_BODY(
		{
			JSFunctionRegister funcs[] =
			{
				JS_FUNCTION_REG(close),
				JS_FUNCTION_REG(setTitle),
				JS_FUNCTION_REG(setSize),
				JS_FUNCTION_REG(title),
				JS_FUNCTION_REG(size),
				JS_FUNCTION_REG_END
			};

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Window, close, JS_BODY(
		{
			Services::Get<WindowService>().Close();
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Window, setTitle, JS_BODY(
		{
			JSWrapper wrapper(args);

			if (wrapper.Check("S") == true)
			{
				Services::Get<WindowService>().SetTitle(wrapper.GetValue<String>(0, "Snuffbox"));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Window, setSize, JS_BODY(
		{
			JSWrapper wrapper(args);

			if (wrapper.Check("NN") == true)
			{
				Services::Get<WindowService>().SetSize(wrapper.GetValue<unsigned int>(0, 1280),
													   wrapper.GetValue<unsigned int>(1, 720));
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Window, title, JS_BODY(
		{
			JSWrapper wrapper(args);
			wrapper.ReturnValue<String>(Services::Get<WindowService>().Title());
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(Window, size, JS_BODY(
		{
			JSWrapper wrapper(args);
			JSWrapper::Object obj = wrapper.CreateObject();

			WindowService& window = Services::Get<WindowService>();

			wrapper.SetObjectValue<unsigned int>(obj, "width", window.Width());
			wrapper.SetObjectValue<unsigned int>(obj, "height", window.Height());

			wrapper.ReturnValue<JSWrapper::Object>(obj);
		}));
	}
}
