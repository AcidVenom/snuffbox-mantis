#include "memory.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Allocator* Memory::default_allocator_ = nullptr;

		//-----------------------------------------------------------------------------------------------
		Allocator& Memory::default_allocator()
		{
			assert(default_allocator_ != nullptr);
			return *default_allocator_;
		}
	}
}
