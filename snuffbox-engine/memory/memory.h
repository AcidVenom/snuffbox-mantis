#pragma once

#include "allocator.h"

namespace snuffbox
{
	namespace engine
	{
		class Application;
		class EASTLAllocator;

		/**
		* @class snuffbox::Memory
		* @brief Provides an interface to access the global memory allocators
		* @author Daniël Konings
		*/
		class Memory
		{

			friend class Application;

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
			* @return (snuffbox::Allocator&) The default allocator
			*/
			static Allocator& default_allocator();

		private:

			static Allocator* default_allocator_; //!< The default allocator
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void Memory::Initialise(const size_t& max_memory)
		{
			assert(default_allocator_ == nullptr);

			static T alloc(max_memory);
			default_allocator_ = &alloc;
		}
	}
}