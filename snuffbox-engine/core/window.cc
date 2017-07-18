#include "window.h"

#include "../services/log_service.h"

#include "GLFW/glfw3.h"

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
		void Window::Initialise(const String& title, const unsigned int& width, const unsigned int& height)
		{
			LogService& log = Services::Get<LogService>();

			log.Assert(glfwInit() == GL_TRUE, "Could not initialise GLFW");
			glfwSetErrorCallback(ErrorCallback);

			/**
			* @todo Turn this on when rendering with DirectX
			*/
			//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			log.Assert((window_ = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr)) != nullptr,
						"GLFW window creation failed");

			title_ = title;
			width_ = width;
			height_ = height;
		}

		//-----------------------------------------------------------------------------------------------
		void Window::SetTitle(const String& title)
		{
			LogService& log = Services::Get<LogService>();
			log.Assert(window_ != nullptr, "Attempted to change the title of a non-existing window");

			glfwSetWindowTitle(window_, title.c_str());
			title_ = title;

			log.Log(console::LogSeverity::kInfo, "Renamed the window title to: {0}", title_.c_str());
		}

		//-----------------------------------------------------------------------------------------------
		void Window::SetSize(const unsigned int& width, const unsigned int& height)
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

			glfwSetWindowSize(window_, width, height);

			width_ = width;
			height_ = height;

			log.Log(console::LogSeverity::kInfo, "Resized the window to: {0}x{1}", width, height);
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
			glfwPollEvents();
		}

		//-----------------------------------------------------------------------------------------------
		void Window::Shutdown()
		{
			Services::Get<LogService>().Assert(window_ != nullptr, "Attempted to shut down the window, but it was never initialised");
			glfwDestroyWindow(window_);
			glfwTerminate();
		}

		//-----------------------------------------------------------------------------------------------
		void Window::ErrorCallback(int error, const char* description)
		{
			Services::Get<LogService>().Log(console::LogSeverity::kError, "GLFW Error: {0} -> {1}", error, description);
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
