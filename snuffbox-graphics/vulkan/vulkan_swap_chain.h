#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace snuffbox
{
	namespace graphics
	{

		class VulkanDisplayDevice;
		class VulkanPhysicalDevice;

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
			* @brief Used to return error codes for swap chain creation
			*/
			enum struct ErrorCode
			{
				kNoSupportedFormats, //!< There are no supported formats
				kNoSupportedPresentModes, //!< The are no supported present modes
				kSuccess //!< Every required feature is supported
			};

			/**
			* @brief Default constructor
			*/
			VulkanSwapChain();

			/**
			* @brief Retrieves the physical device's swap chain capabilities
			* @param[in] device (const VulkanPhysicalDevice*) The physical device to check the capabilities of
			* @param[in] surface (VkSurfaceKHR) The created surface of the display device
			* @return (snuffbox::graphics::VulkanSwapChain::ErrorCode) Does the device have all required swap chain capabilities?
			*/
			ErrorCode GetSupported(const VulkanPhysicalDevice* device, VkSurfaceKHR surface);

			/**
			* @brief Creates the swap chain with the best format and present mode available
			* @param[in] width (unsigned int) The initial width of the swap chain images
			* @param[in] height (unsigned int) The initial height of the swap chain images
			* @param[in] device (const VulkanPhysicalDevice*) The physical device to retrieve the queue indices from
			* @param[in] surface (VkSurfaceKHR) The Vulkan surface created by GLFW
			* @return (bool) Was the creation a success?
			*/
			bool Create(unsigned int width, unsigned int height, const VulkanPhysicalDevice* device, VkSurfaceKHR surface);

			/**
			* @brief Retrieves the best surface format for the listed formats retrieved by snuffbox::graphics::VulkanSwapChain::GetSupported
			* @param[out] out (VkSurfaceFormatKHR*) The best format available
			* @return (bool) Was there a format available to be used?
			*/
			bool BestFormat(VkSurfaceFormatKHR* out) const;

			/**
			* @brief Retrieves the best present mode for the listed present modes retrieved by snuffbox::graphics::VulkanSwapChain::GetSupported
			* @param[out] out (VkPresentModeKHR*) The best present mode available
			* @return (bool) Was there a present mode available to be used?
			*/
			bool BestPresentMode(VkPresentModeKHR* out) const;

			/**
			* @brief Creates the extents for this swap chain
			* @param[in] width (unsigned int) The actual width to set the extents with
			* @param[in] height (unsigned int) The actual height to set the extents with
			* @return (VkExtent2D) The created extents
			*/
			VkExtent2D CreateExtents(unsigned int width, unsigned int height) const;

			/**
			* @brief Creates the image views to access the swap chain images with 
			* @param[in] device (const snuffbox::graphics::VulkanPhysicalDevice*) The physical device created by Vulkan
			* @return (bool) Were we able to create all image views per image?
			*/
			bool CreateImageViews(const VulkanPhysicalDevice* device);

			/**
			* @brief Releases the created swap chain if it was created
			* @param[in] device (const snuffbox::graphics::VulkanPhysicalDevice*) The physical device created by Vulkan
			*/
			void Release(const VulkanPhysicalDevice* device);

		private:

			VkSwapchainKHR handle_; //!< The handle to the actual swap chain to be created with Vulkan

			VkSurfaceCapabilitiesKHR capabilities_; //!< The device capabilities related to the swap chain
			std::vector<VkSurfaceFormatKHR> formats_; //!< The image formats available on the device
			std::vector<VkPresentModeKHR> present_modes_; //!< The present modes availble on the device

			VkSurfaceFormatKHR format_; //!< The best chosen format for this swap chain
			VkExtent2D extents_; //!< The chosen extents for this swap chain

			std::vector<VkImage> images_; //!< A list of the created swap chain images
			std::vector<VkImageView> image_views_; //!< A list of image views to access the created swap chain images with
		};
	}
}