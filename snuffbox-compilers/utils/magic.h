#pragma once

#include <inttypes.h>

namespace snuffbox
{
	namespace compilers
	{
		/**
		* @brief Magic numbers to represent different file types
		*/
		enum struct Magic : uint32_t
		{
			kSnuffboxFile = 0x46554E53, //!< 'SNUF' represented as a hexadecimal value, all snuffbox files should have this in their header
			kScript = 0x00534A53, //!< 'SJS' represented as a hexadecimal value, used for JavaScript files
			kShader = 0x00485353 //!< 'SSH' represented as a hexadecimal value, used for shader files
		};
	}
}