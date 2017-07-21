#pragma once

#include <v8.h>

#include "../memory/memory.h"

namespace snuffbox
{
    namespace engine
    {
        class JSStateWrapper;

        /**
        * @class snuffbox::engine::JSAllocator : public v8::ArrayBuffer::Allocator
        * @brief Used with V8 to handle thread safe memory allocation
        * @author Daniel Konings
        */
        class JSAllocator : public v8::ArrayBuffer::Allocator
        {

            friend class JSStateWrapper;

        protected:

            /**
            * @struct snuffbox::engine::JSAllocator::Header
            * @brief A header necessary for V8 memory allocations
            * @author Daniel Konings
            */
            struct Header
            {
                void* block; //!< A pointer to the actual block
                size_t length; //!< The length of the block
                v8::ArrayBuffer::Allocator::AllocationMode mode; //!< The mode that this block was written with
            };

            /**
            * @brief Construct through a snuffbox allocator
            * @param[in] alloc (snuffbox::engine::Allocator&) The allocator to use in the JavaScript environment, default = snuffbox::engine::Memory::default_allocator()
            */
            JSAllocator(engine::Allocator& alloc = Memory::default_allocator());

            /**
            * @see http://bespin.cz/~ondras/html/classv8_1_1ArrayBuffer_1_1Allocator.html
            */
            void* Allocate(size_t length) override;

            /**
            * @see http://bespin.cz/~ondras/html/classv8_1_1ArrayBuffer_1_1Allocator.html
            */
            void* AllocateUninitialized(size_t length) override;

            /**
            * @see http://bespin.cz/~ondras/html/classv8_1_1ArrayBuffer_1_1Allocator.html
            */
            void Free(void* data, size_t length) override;

        private:

            engine::Allocator& allocator_; //!< The snuffbox allocator used for allocations
        };
    }
}
