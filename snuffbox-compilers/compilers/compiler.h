#pragma once

#include <inttypes.h>
#include <cstdio>
#include <functional>

#include "../utils/magic.h"

namespace snuffbox
{
	namespace compilers
	{
		/**
		* @class snuffbox::compilers::Compiler
		* @brief The base compiler interface that every compiler should inherit from
		* @author Daniel Konings
		*/
		class Compiler
		{

		protected:

			/**
			* @struct snuffbox::compilers::Copmiler::FileHeader
			* @brief Used to retrieve file information from raw input data
			* @author Daniel Konings
			*/
			struct FileHeader
			{
				Magic magic; //!< The magic number to determine that we're actually dealing with Snuffbox files
				Magic file_type; //!< The file type, which should be a unique ID set from each compiler
				size_t file_size; //!< The file size after the file header
			};

		public:

			typedef std::function<void*(size_t)> Allocation;
			typedef std::function<void(void*)> Deallocation;

			/**
			* @brief Setup the compiler with its allocation function
			* @param[in] allocator (snuffbox::compilers::Compiler::Allocation) The allocation function
			* @param[in] deallocator (snuffbox::compilers::Compiler::Deallocation) The deallocation function
			*/
			Compiler(Allocation allocator, Deallocation deallocator);

			/**
			* @see snuffbox::compiler::Compiler::Compilation
			* @param[out] output (const unsigned char**) The compiled data
			* @remarks Makes sure snuffbox::compiler::Compiler::data_ is freed up before re-use
			*/
			bool Compile(const unsigned char* input, size_t size, size_t* out_size, const unsigned char** output, const unsigned char* userdata);

			/**
			* @see snuffbox::compiler::Compiler::Decompilation
			* @param[out] output (const unsigned char**) The decompiled data
			* @param[out] out_size (size_t*) The output size
			* @remarks Makes sure snuffbox::compiler::Compiler::data_ is freed up before re-use
			*/
			bool Decompile(const unsigned char* input, const unsigned char** output, size_t* out_size, const unsigned char* userdata);

			/**
			* @return (const char*) The current error of the compiler, or nullptr if there is none
			*/
			const char* GetError() const;

			/**
			* @brief Frees up the compiled data using snuffbox::compilers::Compiler::deallocator_
			* @remarks As long as snuffbox::compilers::Compiler::data_ is used, all memory will be managed by the compiler
			*/
			~Compiler();

		protected:

			/**
			* @brief Allocates memory using snuffbox::compiler::Compiler::allocator_
			* @param[in] size (size_t) The size to allocate
			* @return (unsigned char*) The allocated data as binary
			* @remarks If snuffbox::compilers::Compiler::allocator_ is nullptr, 'malloc' will be used
			*/
			unsigned char* Allocate(size_t size);

			/**
			* @brief Deallocates memory using snuffbox::compiler::Compiler::deallocator_
			* @param[in] ptr (void*) The pointer to the memory to deallocate
			* @remarks If snuffbox::compilers::Compiler::deallocator_ is nullptr, 'free' will be used
			*/
			void Deallocate(void* ptr);

			/**
			* @brief Compile raw data into a compiled binary format
			* @param[in] input (const unsigned char*) The raw data to compile into the compiled binary format
			* @param[in] size (size_t) The input size
			* @param[out] out_size (size_t*) The output size
			* @param[in] userdata (const unsigned char*) User data to pass into the different compilers
			* @return (bool) Was the compilation a success?
			* @remarks snuffbox::compilers::Compiler::Allocate should be used to allocate the memory for 'out'
			*/
			virtual bool Compilation(const unsigned char* input, size_t size, size_t* out_size, const unsigned char* userdata) = 0;

			/**
			* @brief Decompile compiled binary data into a raw binary format
			* @param[in] input (const unsigned char*) The compiled data to decompile into the raw binary format
			* @param[in] userdata (const unsigned char*) User data to pass into the different compilers
			* @return (bool) Was the decompilation a success?
			* @remarks snuffbox::compilers::Compiler::Allocate should be used to allocate the memory for 'out'
			*/
			virtual bool Decompilation(const unsigned char* input, size_t* out_size, const unsigned char* userdata) = 0;

			/**
			* @brief Sets the current error message of the compiler
			* @param[in] error (const char*) The error message to set
			*/
			void SetError(const char* error);

			/**
			* @brief Retrieves the file header from input data
			* @param[in] input (const unsigned char*) The data to retrieve the file header from
			* @param[in] file_type (snuffbox::compilers::Magic) The expected file type to compare to the 'magic' field of the file header
			* @param[out] out (snuffbox::compilers::Compiler::FileHeader*) The output value
			* @return (bool) Does the raw data contain a file header?
			*/
			bool GetFileHeader(const unsigned char* input, Magic file_type, FileHeader* out);

			/**
			* @brief Creates a file header containing information about the file
			* @param[in] file_size (size_t) The file size after the header
			* @param[in] file_type (snuffbox::compilers::Magic) The type of the file
			*/
			static FileHeader CreateFileHeader(size_t file_size, Magic file_type);

		private:

			Allocation allocator_; //!< The allocation function, used to either allocate the new compiled or raw data
			Deallocation deallocator_; //!< The deallocation function, used to either deallocate the new compiled or raw data

		protected:

			unsigned char* data_; //!< The currently allocated data (compiled or raw)
			char* error_message_; //!< The error message of the compiler, if any
		};
	}
}
