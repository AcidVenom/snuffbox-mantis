#include "vulkan_swap_chain.h"
#include "vulkan_physical_device.h"

#include <algorithm>

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		VulkanSwapChain::VulkanSwapChain() :
			handle_(VK_NULL_HANDLE)
		{

		}

		//-----------------------------------------------------------------------------------------------
		VulkanSwapChain::ErrorCode VulkanSwapChain::GetSupported(const VulkanPhysicalDevice* device, VkSurfaceKHR surface)
		{
			VkPhysicalDevice pd = device->physical_device_;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pd, surface, &capabilities_);

			unsigned int count;
			vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &count, nullptr);

			if (count == 0) 
			{
				return ErrorCode::kNoSupportedFormats;
			}

			formats_.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &count, &formats_[0]);

			vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &count, nullptr);

			if (count == 0) 
			{
				return ErrorCode::kNoSupportedPresentModes;
			}

			present_modes_.resize(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &count, &present_modes_[0]);

			return ErrorCode::kSuccess;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanSwapChain::Create(unsigned int width, unsigned int height, const VulkanPhysicalDevice* device, VkSurfaceKHR surface)
		{
			VkPresentModeKHR present_mode;

			if (BestFormat(&format_) == false)
			{
				return false;
			}

			if (BestPresentMode(&present_mode) == false)
			{
				return false;
			}

			extents_ = CreateExtents(width, height);

			unsigned int images = capabilities_.minImageCount + 1;
			if (capabilities_.maxImageCount > 0 && images > capabilities_.maxImageCount)
			{
				images = capabilities_.maxImageCount;
			}

			VkSwapchainCreateInfoKHR ci = {};
			ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			ci.surface = surface;
			ci.minImageCount = images;
			ci.imageFormat = format_.format;
			ci.imageColorSpace = format_.colorSpace;
			ci.imageExtent = extents_;
			ci.imageArrayLayers = 1;
			ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			unsigned int qf = device->graphics_family_;
			unsigned int pf = device->present_family_;

			if (qf != pf) 
			{
				ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				unsigned int family[] = { qf, pf };

				ci.queueFamilyIndexCount = static_cast<unsigned int>(sizeof(family) / sizeof(unsigned int));
				ci.pQueueFamilyIndices = family;
			}
			else 
			{
				ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				ci.queueFamilyIndexCount = 0;
				ci.pQueueFamilyIndices = nullptr;
			}

			ci.preTransform = capabilities_.currentTransform;
			ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

			ci.presentMode = present_mode;
			ci.clipped = VK_TRUE;

			ci.oldSwapchain = VK_NULL_HANDLE;

			VkDevice ld = device->logical_device_;

			if (vkCreateSwapchainKHR(ld, &ci, nullptr, &handle_) != VK_SUCCESS)
			{
				return false;
			}

			unsigned int count;
			vkGetSwapchainImagesKHR(ld, handle_, &count, nullptr);
			images_.resize(count);
			vkGetSwapchainImagesKHR(ld, handle_, &count, &images_[0]);

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanSwapChain::BestFormat(VkSurfaceFormatKHR* out) const
		{
			if (formats_.size() == 0 || out == nullptr)
			{
				return false;
			}

			if (formats_.size() == 1 && formats_.at(0).format == VK_FORMAT_UNDEFINED) 
			{
				out->format = VK_FORMAT_B8G8R8A8_UNORM;
				out->colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			}

			for (unsigned int i = 0; i < static_cast<unsigned int>(formats_.size()); ++i)
			{
				const VkSurfaceFormatKHR& fmt = formats_.at(i);
				if (fmt.format == VK_FORMAT_B8G8R8A8_UNORM && fmt.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
				{
					*out = fmt;
					return true;
				}
			}

			*out = formats_.at(0);
			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanSwapChain::BestPresentMode(VkPresentModeKHR* out) const
		{
			if (present_modes_.size() == 0 || out == nullptr)
			{
				return false;
			}

			VkPresentModeKHR mode;
			VkPresentModeKHR best = VK_PRESENT_MODE_FIFO_KHR;

			for (unsigned int i = 0; i < static_cast<unsigned int>(present_modes_.size()); ++i) 
			{
				mode = present_modes_.at(i);

				if (mode == VK_PRESENT_MODE_MAILBOX_KHR) 
				{
					*out = mode;
					return true;
				}
				else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) 
				{
					best = mode;
				}
			}

			*out = best;
			return true;
		}

		//-----------------------------------------------------------------------------------------------
		VkExtent2D VulkanSwapChain::CreateExtents(unsigned int width, unsigned int height) const
		{
			if (capabilities_.currentExtent.width != 0xFFFFFFFF) 
			{
				return capabilities_.currentExtent;
			}

			VkExtent2D actual = { width, height };

			actual.width = std::max(capabilities_.minImageExtent.width, std::min(capabilities_.maxImageExtent.width, actual.width));
			actual.height = std::max(capabilities_.minImageExtent.height, std::min(capabilities_.maxImageExtent.height, actual.height));

			return actual;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanSwapChain::Release(const VulkanPhysicalDevice* device)
		{
			if (handle_ != VK_NULL_HANDLE && device != nullptr)
			{
				vkDestroySwapchainKHR(device->logical_device_, handle_, nullptr);
			}
		}
	}
}