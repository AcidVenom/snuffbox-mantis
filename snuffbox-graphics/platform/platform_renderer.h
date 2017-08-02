#pragma once

#include "platform_display_device.h"

namespace snuffbox
{
	namespace graphics
	{
		class Renderer
		{

#ifdef SNUFF_USE_VULKAN
			friend class VulkanDisplayDevice;
			friend class VulkanValidationLayer;
#elif SNUFF_USE_OGL
			friend class OGLDisplayDevice;
#elif SNUFF_USE_D3D11
			friend class D3D11DisplayDevice;
#elif SNUFF_USE_D3D12
			friend class D3D12DisplayDevice;
#endif

		public:

			typedef void(*ErrorCallback)(const char*, bool);

			/**
			* @brief Construct a renderer
			* @param[in] cb (snuffbox::graphics::ErrorCallback) The error callback to log errors with, default = nullptr
			*/
			Renderer(ErrorCallback cb = nullptr);

#ifdef SNUFF_USE_VULKAN

			/**
			* @brief Different initialisation is required for Vulkan, as GLFW needs to pass on its extension compatibility
			* @param[in] ext_count (unsigned int) The number of extensions
			* @param[in] extensions (const char**) The array of extension names
			* @param[in] width (unsigned int) The initial x resolution, in pixels
			* @param[in] height (unsigned int) The initial y resolution, in pixels
			* @return (bool) Was the initialisation of the renderer a success?
			*/
			bool Initialise(unsigned int ext_count, const char** extensions, unsigned int width, unsigned int height);
#else
			/**
			* @brief Initialises the renderer and its display device
			* @param[in] width (unsigned int) The initial x resolution, in pixels
			* @param[in] height (unsigned int) The initial y resolution, in pixels
			*/
			bool Initialise(unsigned int width, unsigned int height);
#endif

			/**
			* @brief Shuts down the renderer
			*/
			void Shutdown();

		protected:

			/**
			* @brief Logs a message using the specified error callback
			* @remarks If snuffbox::graphics::Renderer::error_callback_ is nullptr, the error is ignored
			* @param[in] msg (const char*) The error message
			* @param[in] error (bool) Is this message an error? Default = false
			*/
			void Status(const char* msg, bool error = false) const;

			/**
			* @brief Shorthand for a status message with an error
			* @see snuffbox::graphics::Renderer::Status
			*/
			void Error(const char* msg) const;

		private:

			ErrorCallback error_callback_; //!< The error callback to log errors with
			DisplayDevice display_device_; //!< The platform display device
		};
	}
}