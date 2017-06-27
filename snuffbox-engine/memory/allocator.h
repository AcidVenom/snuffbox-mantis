#pragma once

#include <inttypes.h>
#include <memory>

#include <assert.h>
#include <new>

namespace snuffbox
{
	/**
	* @class snuffbox::Allocator
	* @brief The base class for every memory allocator to use. Keeps track of the current allocations and asserts if there are any leaks
	* @author Dani�l Konings
	*/
	class Allocator
	{

	public:

		/**
		* @brief Construct by specifying the maximum memory size
		* @param[in] max_memory (const size_t&) The maximum memory that can be allocated in bytes
		*/
		Allocator(const size_t& max_memory);

		/**
		* @brief Allocates a block of memory with a given size
		* @remarks This call increases the number of allocations and allocated memory members in the base allocator
		* @param[in] size (const size_t&) The size to allocate
		* @param[in] align (const size_t&) The alignment to allocate with, default = 0
		* @return (void*) A pointer pointing to the address at the start of the block
		*/
		void* Malloc(const size_t& size, const size_t& align = 0);

		/**
		* @brief Frees up a block of memory at a given pointer
		* @param[in] ptr (void*) The pointer pointing to the address at the start of the block
		*/
		void Free(void* ptr);

		/**
		* @brief Constructs an object of type T with arguments Args
		* @remarks This call increases the number of allocations and allocated memory members in the base allocator
		* @param[in] args (Args&&...) The arguments for construction
		* @return (T*) The allocated object
		*/
		template <typename T, typename ... Args>
		T* Construct(const Args... args);

		/**
		* @brief Destructs an object of type T
		* @param[in] ptr (T*) The pointer to the object
		*/
		template <typename T>
		void Destruct(T* ptr);

		/**
		* @brief Checks for any memory left on the heap
		* @remarks This will assert if there are still allocations after destruction, make sure the allocator gets destructed last in the runtime
		*/
		~Allocator();

	protected:

		/**
		* @struct snuffbox::Allocator::Header
		* @brief Contains information about the allocated memory
		* @author Dani�l Konings
		*/
		struct Header
		{
			size_t size; //!< The size of the allocated block of memory
		};

		/**
		* @brief Allocates a block of memory with a specified size and alignment
		* @param[in] size (const size_t&) The size to allocate
		* @param[in] align (const size_t&) The alignment
		* @return (void*) A pointer pointing to the first address of the memory block
		*/
		virtual void* Allocate(const size_t& size, const size_t& align) = 0;

		/**
		* @brief Deallocates a block of memory
		* @param[in] ptr (void*) The pointer pointing to the first address of the memory block to be deallocated
		*/
		virtual void Deallocate(void* ptr) = 0;

	private:

		size_t max_memory_; //!< The maximum allocated memory
		size_t allocated_; //!< The currently allocated memory
		int32_t num_allocations_; //!< The current number of allocations
	};

	//-----------------------------------------------------------------------------------------------
	template <typename T, typename ... Args>
	inline T* Allocator::Construct(const Args... args)
	{
		assert(allocated_ + sizeof(T) <= max_memory_);

		void* allocated = Allocate(sizeof(T), alignof(T));
		T* ptr = new (allocated) T(args...);

		assert(ptr != nullptr);
		
		allocated_ += sizeof(T);
		++num_allocations_;

		return ptr;
	}

	//-----------------------------------------------------------------------------------------------
	template <typename T>
	inline void Allocator::Destruct(T* ptr)
	{
		assert(ptr != nullptr);
		assert(num_allocations_ > 0);
		assert(allocated_ >= sizeof(T));

		ptr->~T();
		Deallocate(ptr);

		allocated_ -= sizeof(T);
		--num_allocations_;
	}
}