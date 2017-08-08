#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace snuffbox
{
	namespace graphics
	{

		class VulkanDisplayDevice;

		/**
		* @class snuffbox::graphics::VulkanSwapChain
		* @brief The swap chain to present images to the screen with
		* @author Daniel Konings
		*/
		class VulkanSwapChain
		{

			friend class VulkanDisplayDevice;

		protected:

			/**
			* @brief Default constructor
			*/
			VulkanSwapChain();

			/**
			* @brief Retrieves the physical device's swap chain capabilities
			*/
			bool CheckSupport(VkPhysicalDevice device);
		private:

			VkSurfaceCapabilitiesKHR capabilities_; //!< The device capabilities related to the swap chain
			std::vector<VkSurfaceFormatKHR> formats_; //!< The image formats available on the device
			std::vector<VkPresentModeKHR> present_modes_; //!< The present modes availble on the device
		};
	}
}