#include "vulkan_display_device.h"
#include "../platform/platform_renderer.h"

namespace snuffbox
{
	namespace graphics
	{

		//-----------------------------------------------------------------------------------------------
		VulkanDisplayDevice::VulkanDisplayDevice(Renderer* renderer) :
			renderer_(renderer),
			instance_(VK_NULL_HANDLE),
			validation_layer_(VulkanValidationLayer::DEFAULT_VALIDATION_LAYER_)
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
		std::vector<const char*> VulkanDisplayDevice::GetRequiredExtensions(unsigned int ext_count, const char** extensions)
		{
			std::vector<const char*> required;

			for (unsigned int i = 0; i < ext_count; i++) 
			{
				required.push_back(extensions[i]);
			}

#ifdef SNUFF_DEBUG
			required.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif
			return required;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanDisplayDevice::CreateInstance(unsigned int ext_count, const char** extensions)
		{
			VkApplicationInfo ai = {};
			ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			ai.pApplicationName = "Snuffbox (Vulkan)";
			ai.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			ai.pEngineName = "snuffbox-mantis";
			ai.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			ai.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo ci = {};
			ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			ci.pApplicationInfo = &ai;

			std::vector<const char*> required_extensions = GetRequiredExtensions(ext_count, extensions);
			ci.enabledExtensionCount = static_cast<unsigned int>(required_extensions.size());
			ci.ppEnabledExtensionNames = &required_extensions[0];

#ifdef SNUFF_DEBUG
			if (validation_layer_.HasValidationLayer() == false)
			{
				renderer_->Error(std::string("The validation layer '" + validation_layer_.name() + "' was requested, but could not be used").c_str());
				return false;
			}

			const char* validation_name = validation_layer_.name().c_str();
			ci.enabledLayerCount = 1;
			ci.ppEnabledLayerNames = &validation_name;

			renderer_->Status(("Enabled Vulkan validation layer: " + validation_layer_.name()).c_str());
#else
			createInfo.enabledLayerCount = 0;
#endif

			VkResult result = vkCreateInstance(&ci, nullptr, &instance_);

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

#ifdef SNUFF_DEBUG
			return validation_layer_.SetupValidationCallback(renderer_, instance_);
#else
			return true;
#endif
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

		//-----------------------------------------------------------------------------------------------
		void VulkanDisplayDevice::Shutdown()
		{
			if (instance_ != VK_NULL_HANDLE)
			{
				validation_layer_.Release(instance_);

				vkDestroyInstance(instance_, nullptr);
				instance_ = VK_NULL_HANDLE;
			}
		}
	}
}