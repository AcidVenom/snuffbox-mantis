#include "vulkan_physical_device.h"

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		VulkanPhysicalDevice VulkanPhysicalDevice::ListDevice(VkPhysicalDevice device)
		{
			VkPhysicalDeviceProperties dp;
			vkGetPhysicalDeviceProperties(device, &dp);

			VkPhysicalDeviceFeatures df;
			vkGetPhysicalDeviceFeatures(device, &df);

			VkPhysicalDeviceMemoryProperties mp;
			vkGetPhysicalDeviceMemoryProperties(device, &mp);

			VulkanPhysicalDevice pd;

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
			properties_.supported = features.geometryShader == VK_TRUE;
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
	}
}