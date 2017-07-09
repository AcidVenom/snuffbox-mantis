#pragma once

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>

#include "../memory/allocators.h"

namespace snuffbox
{
	namespace engine
	{
		typedef eastl::basic_string<char, EASTLAllocator> String;
		
		template <typename T>
		using Vector = eastl::vector<T, EASTLAllocator>;

		template <typename T, typename Y>
		using Map = eastl::map<T, Y, eastl::less<T>, EASTLAllocator>;
	}
}