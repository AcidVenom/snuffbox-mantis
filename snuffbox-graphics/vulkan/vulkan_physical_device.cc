#include "vulkan_physical_device.h"
#include "vulkan_validation_layer.h"

#include <vector>

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice handle) :
			physical_device_(handle),
			logical_device_(VK_NULL_HANDLE),
			queue_family_(-1),
			properties_({ "", 0, false, false }),
			graphics_queue_(VK_NULL_HANDLE)
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		VulkanPhysicalDevice VulkanPhysicalDevice::ListDevice(VkPhysicalDevice device)
		{
			VkPhysicalDeviceProperties dp;
			vkGetPhysicalDeviceProperties(device, &dp);

			VkPhysicalDeviceFeatures df;
			vkGetPhysicalDeviceFeatures(device, &df);

			VkPhysicalDeviceMemoryProperties mp;
			vkGetPhysicalDeviceMemoryProperties(device, &mp);

			VulkanPhysicalDevice pd(device);

			pd.LoadProperties(dp);
			pd.LoadMemory(mp);
			pd.LoadFeatures(df);

			return pd;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanPhysicalDevice::LoadProperties(VkPhysicalDeviceProperties props)
		{
			std::string vendor_name = "";

			switch (props.vendorID)
			{
			case VendorIDs::kAMD:
				vendor_name = "AMD";
				break;

			case VendorIDs::kImgTec:
				vendor_name = "ImgTec";
				break;

			case VendorIDs::kNVIDIA:
				vendor_name = "NVIDIA";
				break;

			case VendorIDs::kARM:
				vendor_name = "ARM";
				break;

			case VendorIDs::kQualcomm:
				vendor_name = "Qualcomm";
				break;

			case VendorIDs::kIntel:
				vendor_name = "Intel";
				break;

			default:
				vendor_name = "Unknown";
				break;
			}

			properties_.name = vendor_name + " - " + props.deviceName;
			properties_.dedicated = props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanPhysicalDevice::LoadMemory(VkPhysicalDeviceMemoryProperties mem)
		{
			properties_.physical_memory = 0;

			VkMemoryHeap heap;
			for (unsigned int i = 0; i < mem.memoryHeapCount; ++i)
			{
				heap = mem.memoryHeaps[i];

				if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
				{
					properties_.physical_memory = heap.size;
					break;
				}
			}
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanPhysicalDevice::LoadFeatures(VkPhysicalDeviceFeatures features)
		{
			queue_family_ = QueueFamilyIndex();
			properties_.supported = queue_family_ >= 0 && features.geometryShader == VK_TRUE;
		}

		//-----------------------------------------------------------------------------------------------
		int VulkanPhysicalDevice::QueueFamilyIndex() const
		{
			unsigned int count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &count, nullptr);

			std::vector<VkQueueFamilyProperties> families(count);
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &count, &families[0]);

			for (unsigned int i = 0; i < count; ++i)
			{
				const VkQueueFamilyProperties& props = families.at(i);

				if (props.queueCount > 0 && props.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
				{
					return static_cast<int>(i);
				}
			}

			return -1;
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int VulkanPhysicalDevice::Rating() const
		{
			if (properties_.supported == false)
			{
				return 0;
			}

			unsigned int rating = 0;

			rating += properties_.dedicated == true ? 1000 : 0;
			rating += static_cast<unsigned int>(properties_.physical_memory / 1024 / 1024);

			return rating;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanPhysicalDevice::Pick(VulkanValidationLayer* vl)
		{
			VkDeviceQueueCreateInfo qci = {};
			qci.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			qci.queueFamilyIndex = queue_family_;
			qci.queueCount = 1;

			float priority = 1.0f;
			qci.pQueuePriorities = &priority;

			VkPhysicalDeviceFeatures req_features = {};
			req_features.geometryShader = VK_TRUE;

			VkDeviceCreateInfo dci = {};
			dci.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			dci.pQueueCreateInfos = &qci;
			dci.queueCreateInfoCount = 1;
			dci.pEnabledFeatures = &req_features;

			if (vl != nullptr)
			{
				vl->Set(dci.enabledLayerCount, dci.ppEnabledLayerNames);
			}

			if (vkCreateDevice(physical_device_, &dci, nullptr, &logical_device_) != VK_SUCCESS)
			{
				return false;
			}

			vkGetDeviceQueue(logical_device_, queue_family_, 0, &graphics_queue_);

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanPhysicalDevice::Release()
		{
			if (logical_device_ != VK_NULL_HANDLE)
			{
				vkDestroyDevice(logical_device_, nullptr);
				logical_device_ = VK_NULL_HANDLE;
			}
		}
	}
}