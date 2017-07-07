#pragma once

#include <EASTL/string.h>
#include "../memory/allocators.h"

namespace snuffbox
{
	namespace engine
	{
		typedef eastl::basic_string<char, EASTLAllocator> String;
	}
}