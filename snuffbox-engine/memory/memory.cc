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

		//-----------------------------------------------------------------------------------------------
		void Memory::Shutdown()
		{
			assert(default_allocator != nullptr);
			default_allocator_->~Allocator();
		}
	}
}