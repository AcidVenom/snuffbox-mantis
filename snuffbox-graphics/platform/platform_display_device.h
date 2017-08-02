#pragma once

#ifdef SNUFF_USE_OGL
	#include "../ogl/ogl_display_device.h"
#elif defined SNUFF_USE_VULKAN
	#include "../vulkan/vulkan_display_device.h"
#elif defined SNUFF_USE_D3D11
	#include "../d3d11/d3d11_display_device.h"
#elif defined SNUFF_USE_D3D12
	#include "../d3d11/d3d12_display_device.h"
#endif

namespace snuffbox
{
	namespace graphics
	{
#ifdef SNUFF_USE_OGL
		typedef OGLDisplayDevice DisplayDevice;
#elif defined SNUFF_USE_VULKAN
		typedef VulkanDisplayDevice DisplayDevice;
#elif defined SNUFF_USE_D3D11
		typedef D3D11DisplayDevice DisplayDevice;
#elif defined SNUFF_USE_D3D12
		typedef D3D12DisplayDevice DisplayDevice;
#endif
	}
}