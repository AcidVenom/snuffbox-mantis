#pragma once

#include "allocator.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::MallocAllocator
		* @brief A default malloc allocator, that uses malloc and free to do its allocations
		* @remarks This allocator however also adds a header to be in-sync with the allocator base
		* @author Daniel Konings
		*/
		class MallocAllocator : public Allocator
		{

		public:

			/**
			* @see snuffbox::engine::Allocator::Allocator
			*/
			MallocAllocator(const size_t& max_memory);

		protected:

			/**
			* @see snuffbox::engine::Allocator::Allocate
			* @remarks Simply a call to 'malloc'
			*/
			void* Allocate(const size_t& size, const size_t& align) override;

			/**
			* @see snuffbox::engine::Allocator::Deallocate
			* @remarks Simply a call to 'free'
			*/
			void Deallocate(void* ptr) override;
		};
	}
}