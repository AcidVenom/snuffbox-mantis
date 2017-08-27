#include "platform_renderer.h"

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		Renderer::Renderer(GLFWwindow* window, ErrorCallback cb) :
			window_(window),
			error_callback_(cb),
			display_device_(this, window)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool Renderer::Initialise(unsigned int width, unsigned int height)
		{
			if (window_ == nullptr)
			{
				return false;
			}

			if (display_device_.Initialise(width, height) == false)
			{
				return false;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool Renderer::CreateShader(const unsigned char* byte_code, size_t size, char type, void** blob)
		{
			return display_device_.CreateShader(byte_code, size, type, blob);
		}

		//-----------------------------------------------------------------------------------------------
		void Renderer::ReleaseShader(void* blob)
		{
			display_device_.ReleaseShader(blob);
		}

		//-----------------------------------------------------------------------------------------------
		void Renderer::Shutdown()
		{
			display_device_.Shutdown();
		}

		//-----------------------------------------------------------------------------------------------
		void Renderer::Status(const char* msg, bool error) const
		{
			if (error_callback_ == nullptr)
			{
				return;
			}

			error_callback_(msg, error);
		}

		//-----------------------------------------------------------------------------------------------
		void Renderer::Error(const char* msg) const
		{
			Status(msg, true);
		}
	}
}