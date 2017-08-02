#include "vulkan_validation_layer.h"
#include "../platform/platform_renderer.h"

#include <string.h>

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		const std::string VulkanValidationLayer::DEFAULT_VALIDATION_LAYER_ = "VK_LAYER_LUNARG_standard_validation";

		//-----------------------------------------------------------------------------------------------
		VulkanValidationLayer::VulkanValidationLayer(const std::string& name, bool verbose) :
			name_(name),
			verbose_(verbose),
			debug_callback_(VK_NULL_HANDLE)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanValidationLayer::HasValidationLayer()
		{
			unsigned int layer_count;
			vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

			validation_layers_.resize(layer_count);
			vkEnumerateInstanceLayerProperties(&layer_count, &validation_layers_[0]);

			for (int i = 0; i < static_cast<int>(validation_layers_.size()); ++i)
			{
				if (strcmp(name_.c_str(), validation_layers_.at(i).layerName) == 0)
				{
					return true;
				}
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanValidationLayer::SetupValidationCallback(Renderer* renderer, VkInstance instance)
		{
			VkDebugReportCallbackCreateInfoEXT ci = {};
			ci.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			
			ci.flags =	VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT | 
						VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT;
			
			if (verbose_ == true)
			{
				ci.flags |=	VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT | 
							VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
			}

			ci.pfnCallback = DebugCallback;
			ci.pUserData = renderer;

			if (CreateDebugReportCallbackEXT(instance, &ci, nullptr, &debug_callback_) != VkResult::VK_SUCCESS)
			{
				renderer->Error(("Could not load the debug callback process address for validation layer '" + name_ + "'").c_str());
				return false;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanValidationLayer::Release(VkInstance instance)
		{
			if (debug_callback_ != VK_NULL_HANDLE)
			{
				DestroyDebugReportCallbackEXT(instance, debug_callback_, nullptr);
				debug_callback_ = VK_NULL_HANDLE;
			}
		}

		//-----------------------------------------------------------------------------------------------
		const std::string& VulkanValidationLayer::name() const
		{
			return name_;
		}

		//-----------------------------------------------------------------------------------------------
		VKAPI_ATTR VkBool32 VKAPI_CALL VulkanValidationLayer::DebugCallback(
			VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData)
		{
			if (userData == nullptr)
			{
				return VK_TRUE;
			}

			std::string message = std::string("[Vulkan VL]: ") + msg;
			reinterpret_cast<Renderer*>(userData)->Status(message.c_str(), (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) == VK_DEBUG_REPORT_ERROR_BIT_EXT);

			return VK_FALSE;
		}

		//-----------------------------------------------------------------------------------------------
		VkResult VulkanValidationLayer::CreateDebugReportCallbackEXT(
			VkInstance instance, 
			const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, 
			const VkAllocationCallbacks* pAllocator, 
			VkDebugReportCallbackEXT* pCallback) 
		{
			PFN_vkCreateDebugReportCallbackEXT func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
			if (func != nullptr) 
			{
				return func(instance, pCreateInfo, pAllocator, pCallback);
			}

			return VkResult::VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanValidationLayer::DestroyDebugReportCallbackEXT(
			VkInstance instance, 
			VkDebugReportCallbackEXT callback, 
			const VkAllocationCallbacks* pAllocator) 
		{
			PFN_vkDestroyDebugReportCallbackEXT func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
			if (func != nullptr) 
			{
				func(instance, callback, pAllocator);
			}
		}
	}
}