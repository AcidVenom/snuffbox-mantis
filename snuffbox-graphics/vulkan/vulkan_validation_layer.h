#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

namespace snuffbox
{
	namespace graphics
	{
		class Renderer;
		class VulkanDisplayDevice;

		/**
		* @class snuffbox::graphics::VulkanValidationLayer
		* @brief A helper class to setup a Vulkan validation layer by name
		* @author Daniel Konings
		*/
		class VulkanValidationLayer
		{
			friend class VulkanDisplayDevice;

		protected:

			/**
			* @brief Creates a validation layer by name
			* @param[in] name (const std::string&) The name of the validation layer
			* @param[in] verbose (bool) Should the validation layer do verbose logging? (e.g. Debug and information messages as well), default = false
			*/
			VulkanValidationLayer(const std::string& name, bool verbose = false);

			/**
			* @return (bool) Is the validation layer with the current name available?
			*/
			bool HasValidationLayer();

			/**
			* @brief Sets up the validation layer callback
			* @param[in] renderer (snuffbox::graphics::Renderer*) The renderer to send the messages to
			* @param[in] instance (VkInstance) The instance to setup the callback with
			* @return (bool) Were we able to setup the validation callback?
			*/
			bool SetupValidationCallback(Renderer* renderer, VkInstance instance);

			/**
			* @brief Releases all created objects
			* @param[in] instance (VkInstance) The Vulkan instance
			*/
			void Release(VkInstance instance);

			/**
			* @return (const std::string&) The name of this validation layer
			*/
			const std::string& name() const;

			static const std::string DEFAULT_VALIDATION_LAYER_; //!< The name of the default validation layer provided by the LunarG SDK

			/**
			* @see https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Validation_layers
			*/
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugReportFlagsEXT flags,
				VkDebugReportObjectTypeEXT objType,
				uint64_t obj,
				size_t location,
				int32_t code,
				const char* layerPrefix,
				const char* msg,
				void* userData);

			/**
			* @see https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Validation_layers
			*/
			static VkResult CreateDebugReportCallbackEXT(
				VkInstance instance,
				const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
				const VkAllocationCallbacks* pAllocator,
				VkDebugReportCallbackEXT* pCallback);

			/**
			* @see https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Validation_layers
			*/
			static void DestroyDebugReportCallbackEXT(
				VkInstance instance,
				VkDebugReportCallbackEXT callback,
				const VkAllocationCallbacks* pAllocator);

		private:

			std::string name_; //!< The name of this validation layer
			bool verbose_; //!< Should verbose logging be enabled?

			std::vector<VkLayerProperties> validation_layers_; //!< The available validation layers

			VkDebugReportCallbackEXT debug_callback_; //!< The loaded debug callback

			VkPhysicalDevice gpu_; //!< The picked physical device for rendering
		};
	}
}