#pragma once

#include "vulkan_validation_layer.h"

namespace snuffbox
{
	namespace graphics
	{
		class Renderer;

		/**
		* @class snuffbox::graphics::VulkanDisplayDevice
		* @brief The display device when Snuffbox is ran with Vulkan
		* @author Daniel Konings
		*/
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
			* @brief Retrieves the required extensions to create a Vulkan instance
			* @param[in] ext_count (unsigned int) The number of extensions required by GLFW
			* @param[in] extensions (const char**) The array of extension names required by GLFW
			* @return (std::vector<const char*>) A list of the initial required extensions
			*/
			std::vector<const char*> GetRequiredExtensions(unsigned int ext_count, const char** extensions);

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

			/**
			* @brief Cleans up Vulkan
			*/
			void Shutdown();

		private:

			Renderer* renderer_; //!< The renderer this device was constructed from
			VkInstance instance_; //!< The Vulkan instance

			std::vector<VkExtensionProperties> extensions_; //!< The extensions available for this Vulkan instance

			VulkanValidationLayer validation_layer_; //!< The validation layer to use in debug builds
		};
	}
}
