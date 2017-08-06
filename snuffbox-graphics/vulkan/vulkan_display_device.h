#pragma once

#include "vulkan_validation_layer.h"
#include "vulkan_physical_device.h"

struct GLFWwindow;

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
			* @param[in] window (GLFWwindow*) The window to output to
			*/
			VulkanDisplayDevice(Renderer* renderer, GLFWwindow* window);

			/**
			* @brief Initialises the display device with the extensions retrieved from GLFW
			* @param[in] width (unsigned int) The initial width to create the display device with, in pixels
			* @param[in] height (unsigned int) The initial height to create the display device with, in pixels
			* @return (bool) Was the initialisation a success?
			*/
			bool Initialise(unsigned int width, unsigned int height);

			/**
			* @brief Retrieves the required extensions to create a Vulkan instance
			* @param[in] ext_count (unsigned int) The number of extensions required by GLFW
			* @param[in] extensions (const char**) The array of extension names required by GLFW
			* @return (std::vector<const char*>) A list of the initial required extensions
			*/
			std::vector<const char*> GetRequiredExtensions(unsigned int ext_count, const char** extensions);

			/**
			* @brief Creates the Vulkan instance
			* @return (bool) Was the instance succesfully created?
			*/
			bool CreateInstance();

			/**
			* @brief Retrieves the list of extensions
			*/
			void GetExtensions();

			/**
			* @brief Retrieves the list of physical devices and enumerates their data
			*/
			bool GetPhysicalDevices();

			/**
			* @brief Logs the device properties of a physical device in the devices list
			* @param[in] idx (unsigned int) The index of the physical device to log the properties of
			*/
			void LogDeviceProperties(unsigned int idx);

			/**
			* @brief Creates the window surface with GLFW
			* @return (bool) Was the window surface created succesfully?
			*/
			bool CreateWindowSurface();

			/**
			* @brief Cleans up Vulkan
			*/
			void Shutdown();

		private:

			Renderer* renderer_; //!< The renderer this device was constructed from
			GLFWwindow* window_; //!< The window to output to

			VkInstance instance_; //!< The Vulkan instance

			std::vector<VkExtensionProperties> extensions_; //!< The extensions available for this Vulkan instance
			VulkanValidationLayer validation_layer_; //!< The validation layer to use in debug builds

			std::vector<VulkanPhysicalDevice> physical_devices_; //!< The physical devices and their properties/features

			VulkanPhysicalDevice* device_; //!< The picked physical device for rendering

			VkSurfaceKHR surface_; //!< The Window surface created by GLFW
		};
	}
}
