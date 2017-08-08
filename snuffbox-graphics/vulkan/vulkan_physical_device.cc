#include "vulkan_physical_device.h"
#include "vulkan_validation_layer.h"

#include <vector>

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		const char* VulkanPhysicalDevice::REQUIRED_EXTENSIONS_[VulkanPhysicalDevice::EXTENSION_COUNT_] =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		//-----------------------------------------------------------------------------------------------
		VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice handle) :
			physical_device_(handle),
			logical_device_(VK_NULL_HANDLE),
			graphics_family_(-1),
			properties_({ "", 0, false, false }),
			graphics_queue_(VK_NULL_HANDLE),
			present_queue_(VK_NULL_HANDLE)
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
			graphics_family_ = QueueFamilyIndex([](const VkQueueFamilyProperties& props, int i)
			{
				return (props.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT;
			});

			bool ext = HasRequiredExtensions();

			properties_.supported = 
				ext == true && 
				graphics_family_ >= 0 && 
				features.geometryShader == VK_TRUE;
		}

		//-----------------------------------------------------------------------------------------------
		int VulkanPhysicalDevice::QueueFamilyIndex(const std::function<bool(const VkQueueFamilyProperties&, int i)>& condition) const
		{
			if (condition == nullptr)
			{
				return -1;
			}

			unsigned int count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &count, nullptr);

			std::vector<VkQueueFamilyProperties> families(count);
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &count, &families[0]);

			for (unsigned int i = 0; i < count; ++i)
			{
				const VkQueueFamilyProperties& props = families.at(i);

				if (props.queueCount > 0 && condition(props, i) == true) 
				{
					return static_cast<int>(i);
				}
			}

			return -1;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanPhysicalDevice::HasRequiredExtensions() const
		{
			unsigned int ext_count;
			vkEnumerateDeviceExtensionProperties(physical_device_, nullptr, &ext_count, nullptr);

			std::vector<VkExtensionProperties> available(ext_count);
			vkEnumerateDeviceExtensionProperties(physical_device_, nullptr, &ext_count, &available[0]);

			bool found;
			for (unsigned int i = 0; i < EXTENSION_COUNT_; ++i)
			{
				found = false;
				for (unsigned int j = 0; j < available.size(); ++j)
				{
					if (strcmp(REQUIRED_EXTENSIONS_[i], available.at(j).extensionName) == 0)
					{
						found = true;
						break;
					}
				}

				if (found == false)
				{
					return false;
				}
			}

			return true;
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
		bool VulkanPhysicalDevice::Pick(VkSurfaceKHR surface, VulkanValidationLayer* vl)
		{
			if (GetPresentSupport(surface) == false)
			{
				return false;
			}

			int queues[] = 
			{
				graphics_family_,
				present_family_
			};

			bool added[16];

			for (int i = 0; i < 16; ++i)
			{
				added[i] = false;
			}

			unsigned int count = static_cast<unsigned int>(sizeof(queues) / sizeof(int));

			std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

			VkDeviceQueueCreateInfo qci;
			float priority = 1.0f;

			for (unsigned int i = 0; i < count; ++i)
			{
				if (added[queues[i]] == true)
				{
					continue;
				}

				qci = {};
				qci.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				qci.queueFamilyIndex = queues[i];
				qci.queueCount = 1;
				qci.pQueuePriorities = &priority;

				queue_create_infos.push_back(qci);
				added[qci.queueFamilyIndex] = true;
			}

			VkPhysicalDeviceFeatures req_features = {};
			req_features.geometryShader = VK_TRUE;

			VkDeviceCreateInfo dci = {};
			dci.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			dci.pQueueCreateInfos = &queue_create_infos[0];
			dci.queueCreateInfoCount = static_cast<unsigned int>(queue_create_infos.size());
			dci.pEnabledFeatures = &req_features;
			dci.enabledExtensionCount = EXTENSION_COUNT_;
			dci.ppEnabledExtensionNames = REQUIRED_EXTENSIONS_;

			if (vl != nullptr)
			{
				vl->Set(dci.enabledLayerCount, dci.ppEnabledLayerNames);
			}

			if (vkCreateDevice(physical_device_, &dci, nullptr, &logical_device_) != VK_SUCCESS)
			{
				return false;
			}

			vkGetDeviceQueue(logical_device_, graphics_family_, 0, &graphics_queue_);
			vkGetDeviceQueue(logical_device_, present_family_, 0, &present_queue_);

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanPhysicalDevice::GetPresentSupport(VkSurfaceKHR surface)
		{
			present_family_ = QueueFamilyIndex([=](const VkQueueFamilyProperties& props, int i)
			{
				unsigned int supported;
				vkGetPhysicalDeviceSurfaceSupportKHR(physical_device_, i, surface, &supported);

				return supported == VK_TRUE;
			});

			return present_family_ >= 0;
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