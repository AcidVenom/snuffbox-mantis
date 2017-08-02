#include "platform_renderer.h"

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		Renderer::Renderer(ErrorCallback cb) :
			error_callback_(cb),
			display_device_(this)
		{

		}

#ifdef SNUFF_USE_VULKAN
		//-----------------------------------------------------------------------------------------------
		bool Renderer::Initialise(unsigned int ext_count, const char** extensions, unsigned int width, unsigned int height)
		{
			if (display_device_.Initialise(ext_count, extensions) == false)
			{
				return false;
			}

			return true;
		}

#else
		//-----------------------------------------------------------------------------------------------
		bool Renderer::Initialise()
		{

		}
#endif

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