#include "eastl_allocator.h"
#include "../memory/memory.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		EASTLAllocator::EASTLAllocator(const char* pName) :
			allocator_(EASTL_ALLOCATOR)
		{
#if EASTL_NAME_ENABLED
			mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
#endif
		}

		//-----------------------------------------------------------------------------------------------
		EASTLAllocator::EASTLAllocator(const eastl::allocator& x, const char* pName) :
			allocator_(Memory::default_allocator())
		{
#if EASTL_NAME_ENABLED
			mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
#endif
		}

		//-----------------------------------------------------------------------------------------------
		EASTLAllocator::~EASTLAllocator()
		{

		}

		//-----------------------------------------------------------------------------------------------
		EASTLAllocator& EASTLAllocator::operator=(const EASTLAllocator& x)
		{
			return *this;
		}

		//-----------------------------------------------------------------------------------------------
		void* EASTLAllocator::allocate(size_t n, int flags)
		{
			return allocator_.Malloc(n);
		}

		//-----------------------------------------------------------------------------------------------
		void* EASTLAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
		{
			return allocator_.Malloc(n);
		}

		//-----------------------------------------------------------------------------------------------
		void EASTLAllocator::deallocate(void* p, size_t n)
		{
			allocator_.Free(p);
		}

		//-----------------------------------------------------------------------------------------------
		void EASTLAllocator::set_name(const char* pName)
		{
#if EASTL_NAME_ENABLED
			mpName = pName;
#endif
		}

		//-----------------------------------------------------------------------------------------------
		const char* EASTLAllocator::get_name() const
		{
#if EASTL_NAME_ENABLED
			return mpName;
#else
			return "Custom EASTL allocator";
#endif
		}
	}
}