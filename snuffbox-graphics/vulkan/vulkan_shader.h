#pragma once

#include <vulkan/vulkan.h>

namespace snuffbox
{
	namespace graphics
	{
		class VulkanDisplayDevice;
		class VulkanPhysicalDevice;

		/**
		* @class snuffbox::engine::VulkanShader
		* @brief A wrapper around a platform-specific shader when created in Vulkan
		* @author Daniel Konings
		*/
		class VulkanShader
		{

			friend class VulkanDisplayDevice;

		protected:

			/**
			* @brief Default constructor
			*/
			VulkanShader();

			/**
			* @brief Creates a Spir-V shader in GPU memory
			* @param[in] byte_code (const unsigned char*) The Spir-V byte code
			* @param[in] size (size_t) The size of the byte code
			* @param[in] type (char) The type of shader
			* @param[in] device (snuffbox::graphics::VulkanPhysicalDevice*) The device to create the shader with
			* @return (bool) Was it a success?
			*/
			bool Create(const unsigned char* byte_code, size_t size, char type, VulkanPhysicalDevice* device);

			/**
			* @brief Releases the shader
			* @param[in] device (snuffbox::graphics::VulkanPhysicalDevice*) The device to release the shader with
			*/
			void Release(VulkanPhysicalDevice* device);

		private:

			VkShaderModule module_; //!< The module of this shader
			VkPipelineShaderStageCreateInfo staging_; //!< The staging create info
		};
	}
}