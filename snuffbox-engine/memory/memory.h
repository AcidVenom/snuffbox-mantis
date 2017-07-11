#pragma once

#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>

#include "eastl_allocator.h"

namespace snuffbox
{
	namespace engine
	{
		class SnuffboxApp;
		class EASTLAllocator;

		template <typename T>
		struct EASTLDeleter;

		template <typename T>
		using UniquePtr = eastl::unique_ptr<T, EASTLDeleter<T>>;

		template <typename T>
		using SharedPtr = eastl::shared_ptr<T>;

		/**
		* @class snuffbox::Memory
		* @brief Provides an interface to access the global memory allocators
		* @author Daniël Konings
		*/
		class Memory
		{

			friend class SnuffboxApp;

		protected:

			/**
			* @brief Initialises the memory system with an allocator of type T
			* @param[in] max_memory (const size_t&) The maximum memory that can be allocated
			*/
			template <typename T>
			static void Initialise(const size_t& max_memory);

			/**
			* @brief Called at the end of the application to shutdown
			*/
			static void Shutdown();

		public:

			/**
			* @brief Constructs a shared pointer with the EASTL allocator
			* @param[in] args (const Args&...) The arguments to pass to the constructor
			* @remarks Classes that will be constructed through this interface require friendship with snuffbox::engine::Allocator
			* @return (snuffbox::engine::SharedPtr<T>) The constructed shared pointer
			*/
			template <typename T, typename ... Args>
			static SharedPtr<T> ConstructShared(const Args&... args);

			/**
			* @brief Constructs a unique pointer with the EASTL allocator
			* @param[in] args (const Args&...) The arguments to pass to the constructor
			* @remarks Classes that will be constructed through this interface require friendship with snuffbox::engine::Allocator
			* @return (snuffbox::engine::UniquePtr<T>) The constructed unique pointer
			*/
			template <typename T, typename ... Args>
			static UniquePtr<T> ConstructUnique(const Args&... args);

			/**
			* @return (snuffbox::Allocator&) The default allocator
			*/
			static Allocator& default_allocator();

		private:

			static Allocator* default_allocator_; //!< The default allocator
		};

		/**
		* @struct snuffbox::engine::EASTLDeleter<T>
		* @brief Deletes pointers stored in EASTL smart pointers
		* @author Daniël Konings
		*/
		template <typename T>
		struct EASTLDeleter
		{
			/**
			* @brief The pointer deletion function
			* @param[in] ptr (T*) The pointer to delete
			*/
			void operator()(T* ptr);
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void Memory::Initialise(const size_t& max_memory)
		{
			assert(default_allocator_ == nullptr);

			static T alloc(max_memory);
			default_allocator_ = &alloc;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T, typename ... Args>
		inline SharedPtr<T> Memory::ConstructShared(const Args&... args)
		{
			T* ptr = EASTL_ALLOCATOR.Construct<T>(args...);
			return eastl::shared_ptr<T>(ptr, EASTLDeleter<T>(), EASTLAllocator());
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T, typename ... Args>
		inline UniquePtr<T> Memory::ConstructUnique(const Args&... args)
		{
			T* ptr = EASTL_ALLOCATOR.Construct<T>(args...);
			return UniquePtr<T>(ptr);
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void EASTLDeleter<T>::operator()(T* ptr)
		{
			if (ptr == nullptr)
			{
				return;
			}
			
			EASTL_ALLOCATOR.Destruct(ptr);
		}
	}
}