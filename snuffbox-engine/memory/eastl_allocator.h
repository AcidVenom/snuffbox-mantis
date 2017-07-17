#pragma once

#include <EASTL/allocator.h>
#include "allocator.h"

#define EASTL_ALLOCATOR Memory::default_allocator()

namespace snuffbox
{
	namespace engine
	{
		class EASTLAllocator
		{
		public:

			/**
			* @brief Construct an EASTL allocator with a name
			* @param[in] pName (const char*) The name for debugging
			*/
			EASTLAllocator(const char* pName = EASTL_NAME_VAL("EASTLAllocator"));

			/**
			* @brief Copy constructor
			* @param[in] x (const eastl::allocator&) The allocator to copy from
			* @param[in] pName (const char*) The name for debugging
			*/
			EASTLAllocator(const eastl::allocator& x, const char* pName = EASTL_NAME_VAL("EASTLAllocator"));

			/**
			* @brief Default destructor
			*/
			~EASTLAllocator();

			/**
			* @brief Copy assignment operaetor
            * @param[in] x (const snuffbox::engine::EASTLAllocator&) The allocator to copy from
			*/
			EASTLAllocator& operator=(const EASTLAllocator& x);

            /**
            * @brief Comparison operator, always returns true
            * @param[in] x (const snuffbox::engine::EASTLAllocator&) The allocator to compare against
            * @return (bool) Always true, as there is only 1 EASTLAllocator
            */
            bool operator==(const EASTLAllocator& x);

			/**
			* @brief Allocate with a given size
			* @param[in] n (size_t) The size to allocate
			* @param[in] flags (int) EASTL allocator flags, default = 0
			* @remarks This uses the underlying snuffbox allocator T
			* @return (void*) The pointer pointing to the allocated memory block
			*/
			void* allocate(size_t n, int flags = 0);

			/**
			* @brief Allocate with a given size and alignment
			* @param[in] n (size_t) The size to allocate
			* @param[in] alignment (size_t) The alignment to use
			* @param[in] offset (size_t) The offset to use, unused right now
			* @param[in] flags (int) EASTL allocator flags, default = 0
			* @remarks This uses the underlying snuffbox allocator T
			* @return (void*) The pointer pointing to the allocated memory block
			*/
			void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);

			/**
			* @brief Deallocates memory at a provided pointer
			* @param[in] p (void*) The pointer to deallocate the memory from
			* @param[in] n (size_t) The size to deallocate
			*/
			void deallocate(void* p, size_t n);

			/**
			* @return (const char*) Returns the debug name of this allocator
			*/
			const char* get_name() const;

			/**
			* @brief Sets the debug name of this allocator
			* @param[in] pName (const char*) The name to set
			*/
			void set_name(const char* pName);

		protected:

#if EASTL_NAME_ENABLED
			const char* mpName; //!< The debug name of this allocator
#endif
			Allocator& allocator_; //!< A reference to the memory's default allocator
		};
	}
}
