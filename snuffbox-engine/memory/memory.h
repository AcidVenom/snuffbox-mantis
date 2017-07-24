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
		* @author Daniel Konings
		*/
		class Memory
		{

			friend class SnuffboxApp;

		protected:

			/**
			* @brief Initialises the memory system with an allocator of type T
			* @param[in] max_memory (size_t) The maximum memory that can be allocated
			*/
			template <typename T>
			static void Initialise(size_t max_memory);

		public:

			/**
			* @brief Constructs a shared pointer with the EASTL allocator
			* @param[in] args (Args&&...) The arguments to pass to the constructor
			* @remarks Classes that will be constructed through this interface require friendship with snuffbox::engine::Allocator
			* @return (snuffbox::engine::SharedPtr<T>) The constructed shared pointer
			*/
			template <typename T, typename ... Args>
			static SharedPtr<T> ConstructShared(Args&&... args);

			/**
			* @brief Makes a raw pointer a shared pointer
			* @param[in] ptr (T*) The raw pointer
			* @return (snuffbox::engine::SharedPtr<T>) The constructed shared pointer
			*/
			template <typename T>
			static SharedPtr<T> MakeShared(T* ptr);

			/**
			* @brief Constructs a unique pointer with the EASTL allocator
			* @param[in] args (Args&&...) The arguments to pass to the constructor
			* @remarks Classes that will be constructed through this interface require friendship with snuffbox::engine::Allocator
			* @return (snuffbox::engine::UniquePtr<T>) The constructed unique pointer
			*/
			template <typename T, typename ... Args>
			static UniquePtr<T> ConstructUnique(Args&&... args);

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
		* @author Daniel Konings
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
		inline void Memory::Initialise(size_t max_memory)
		{
			assert(default_allocator_ == nullptr);

			static T alloc(max_memory);
			default_allocator_ = &alloc;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T, typename ... Args>
		inline SharedPtr<T> Memory::ConstructShared(Args&&... args)
		{
			T* ptr = EASTL_ALLOCATOR.Construct<T>(std::forward<Args>(args)...);
			return eastl::shared_ptr<T>(ptr, EASTLDeleter<T>(), EASTLAllocator());
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline SharedPtr<T> Memory::MakeShared(T* ptr)
		{
			return eastl::shared_ptr<T>(ptr, EASTLDeleter<T>(), EASTLAllocator());
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T, typename ... Args>
		inline UniquePtr<T> Memory::ConstructUnique(Args&&... args)
		{
			T* ptr = EASTL_ALLOCATOR.Construct<T>(std::forward<Args>(args)...);
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
