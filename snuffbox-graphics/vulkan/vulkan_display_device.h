#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace snuffbox
{
	namespace graphics
	{
		class Renderer;

		class VulkanDisplayDevice
		{

			friend class Renderer;

		protected:

			/**
			* @brief Construct from a renderer
			* @param[in] renderer (snuffbox::graphics::Renderer*) The renderer this device was constructed from
			*/
			VulkanDisplayDevice(Renderer* renderer);

			/**
			* @brief Initialises the display device with the extensions retrieved from GLFW
			* @param[in] ext_count (unsigned int) The number of extensions
			* @param[in] extensions (const char**) The array of extension names
			* @return (bool) Was the initialisation a success?
			*/
			bool Initialise(unsigned int ext_count, const char** extensions);

			/**
			* @brief Creates the Vulkan instance
			* @see snuffbox::graphics::VulkanDisplayDevice::Initialise
			* @return (bool) Was the instance succesfully created?
			*/
			bool CreateInstance(unsigned int ext_count, const char** extensions);

			/**
			* @brief Retrieves the list of extensions
			*/
			void GetExtensions();

		private:

			Renderer* renderer_; //!< The renderer this device was constructed from
			VkInstance instance_; //!< The Vulkan instance

			std::vector<VkExtensionProperties> extensions_; //!< The extensions available for this Vulkan instance
		};
	}
}
