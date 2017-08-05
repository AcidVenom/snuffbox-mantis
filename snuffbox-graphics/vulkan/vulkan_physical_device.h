#pragma once

#include <string>

#include <vulkan/vulkan.h>

namespace snuffbox
{
	namespace graphics
	{
		class VulkanDisplayDevice;

		/**
		* @class snuffbox::graphics::VulkanPhysicalDevice
		* @brief A helper class to enumerate the different physical devices available on the system
		* @author Daniel Konings
		*/
		class VulkanPhysicalDevice
		{

			friend class VulkanDisplayDevice;

		public:

			/**
			* @brief The vendor IDs that are available in the current Vulkan specification
			*/
			enum VendorIDs
			{
				kAMD = 0x1002, //!< AMD
				kImgTec = 0x1010, //!< ImgTec
				kNVIDIA = 0x10DE, //!< NVIDIA
				kARM = 0x13B5, //!< ARM
				kQualcomm = 0x5143, //!< Qualcomm
				kIntel = 0x8086 //!< Intel
			};

			/**
			* @struct snuffbox::graphics::VulkanPhysicalDevice::Properties
			* @brief A structure to define the different properties of a physical device
			* @author Daniel Konings
			*/
			struct Properties
			{
				std::string name; //!< The name of the device
				size_t physical_memory; //!< The physical memory of the device
				bool dedicated; //!< Is this a dedicated GPU?
				bool supported; //!< Does this device support all Snuffbox features?
			};

		protected:

			/**
			* @brief Lists this device and its properties from the list of physical devices retrieved from Vulkan
			* @param[in] device (VkPhysicalDevice) The Vulkan handle to enumerate the properties of
			* @return (snuffbox::graphics::VulkanPhysicalDevice) The physical device and its properties
			*/
			static VulkanPhysicalDevice ListDevice(VkPhysicalDevice device);

			/**
			* @brief Loads the properties of this device from the corresponding Vulkan handle
			* @param[in] props (VkPhysicalDeviceProperties) The properties handle to load from
			*/
			void LoadProperties(VkPhysicalDeviceProperties props);

			/**
			* @brief Loads the memory properties of this device from the corresponding Vulkan handle
			* @param[in] mem (VkPhysicalDeviceMemoryProperties) The memory properties handle to load from
			*/
			void LoadMemory(VkPhysicalDeviceMemoryProperties mem);

			/**
			* @brief Loads the features of this device from the corresponding Vulkan handle
			* @param[in] features (VkPhysicalDeviceFeatures) The features handle to load from
			*/
			void LoadFeatures(VkPhysicalDeviceFeatures features);

			/**
			* @brief Retrieves the 'rating' of this physical device to see how good it is opposed to other physical devices
			* @return (unsigned int) The rating of this physical devices
			* @remarks This is currently based on whether the this device is a dedicated GPU or not, its physical memory in megabytes and if it is supported
			*/
			unsigned int Rating() const;

		private:

			Properties properties_; //!< The properties of this physical device
			VkPhysicalDevice handle_; //!< The handle to the physical device this device was created with
		};
	}
}