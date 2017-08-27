#include "vulkan_shader.h"
#include "vulkan_physical_device.h"

#include <snuffbox-compilers/utils/glslang_validator.h>

namespace snuffbox
{
	namespace graphics
	{
		//-----------------------------------------------------------------------------------------------
		VulkanShader::VulkanShader() :
			module_(VK_NULL_HANDLE)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool VulkanShader::Create(const unsigned char* byte_code, size_t size, char type, VulkanPhysicalDevice* device)
		{
			VkShaderModuleCreateInfo ci = {};
			ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			ci.codeSize = size;
			ci.pCode = reinterpret_cast<const uint32_t*>(byte_code);

			if (vkCreateShaderModule(device->logical_device_, &ci, nullptr, &module_) != VK_SUCCESS)
			{
				return false;
			}

			VkPipelineShaderStageCreateInfo sci = {};
			sci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			sci.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			sci.module = module_;
			sci.pName = "main";

			VkShaderStageFlagBits stage;

			switch (static_cast<compilers::GLSLangValidator::ShaderType>(type))
			{
			case compilers::GLSLangValidator::ShaderType::kVertex:
				stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
				break;

			case compilers::GLSLangValidator::ShaderType::kPixel:
				stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
				break;

			case compilers::GLSLangValidator::ShaderType::kGeometry:
				stage = VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
				break;

			default:
				Release(device);
				return false;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void VulkanShader::Release(VulkanPhysicalDevice* device)
		{
			if (module_ != VK_NULL_HANDLE)
			{
				vkDestroyShaderModule(device->logical_device_, module_, nullptr);
				module_ = VK_NULL_HANDLE;
			}
		}
	}
}