#include "vulkan_display_device.h"
#include "../platform/platform_renderer.h"

#include <string>

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		VulkanDisplayDevice::VulkanDisplayDevice(Renderer* renderer) :
			renderer_(renderer),
			instance_(nullptr)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanDisplayDevice::Initialise(unsigned int ext_count, const char** extensions)
		{
			if (CreateInstance(ext_count, extensions) == false)
			{
				return false;
			}

			GetExtensions();

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanDisplayDevice::CreateInstance(unsigned int ext_count, const char** extensions)
		{
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Snuffbox - Vulkan";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "snuffbox-mantis";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = ext_count;
			createInfo.ppEnabledExtensionNames = extensions;
			createInfo.enabledLayerCount = 0;

			VkResult result = vkCreateInstance(&createInfo, nullptr, &instance_);

			if (result != VkResult::VK_SUCCESS)
			{
				switch (result)
				{
				case VkResult::VK_ERROR_OUT_OF_HOST_MEMORY:
					renderer_->Error("Could not create a Vulkan instance, out of host memory");
					break;

				case VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY:
					renderer_->Error("Could not create a Vulkan instance, out of device memory");
					break;

				case VkResult::VK_ERROR_INITIALIZATION_FAILED:
					renderer_->Error("Could not create a Vulkan instance, initialisation failed");
					break;

				case VkResult::VK_ERROR_LAYER_NOT_PRESENT:
					renderer_->Error("Could not create a Vulkan instance, the validation layer is not present");
					break;

				case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT:
					renderer_->Error("Could not create a Vulkan instance, an extension is not present");
					break;

				case VkResult::VK_ERROR_INCOMPATIBLE_DRIVER:
					renderer_->Error("Could not create a Vulkan instance, there is no compatible driver");
					break;

				default:
					renderer_->Error("Could not create a Vulkan instance, unknown error");
					break;
				}
				
				return false;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanDisplayDevice::GetExtensions()
		{
			uint32_t ext_count = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

			extensions_.resize(ext_count);
			vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, &extensions_[0]);

			renderer_->Status("Available Vulkan extensions:");
			std::string name = "\t";

			for (int i = 0; i < static_cast<int>(extensions_.size()); ++i)
			{
				renderer_->Status((name + extensions_.at(i).extensionName).c_str());
			}
		}
	}
}