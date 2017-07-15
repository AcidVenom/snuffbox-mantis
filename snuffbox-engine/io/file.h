#pragma once

#include <stdio.h>

#include "../core/eastl.h"
#include <time.h>

#ifdef SNUFF_WIN32
#define fopen(out, path, flags) fopen_s(&out, path, flags);
#define fread(out, size, file) fread_s(out, size, size, 1, file)
#else
#define fopen(out, path, flags) out = fopen(path, flags);
#define fread(out, size, file) fread(&out, size, 1, file);
#endif

namespace snuffbox
{
	namespace engine
	{
		class Allocator;

		/**
		* @class snuffbox::engine::File
		* @brief A helper class for cross-platform file reading/writing
		* @author Daniël Konings
		*/
		class File
		{

			friend class Allocator;

		protected:

			/**
			* @brief Default constructor
			*/
			File();

			/**
			* @brief Remove copy constructor from public scope
			*/
			File(const File& other) = default;

			/**
			* @brief Reads all the file data into this class' buffer
			*/
			void Read();

		public:

			/**
			* @brief Bit-shiftable access flags for the access mode of the file
			*/
			enum AccessFlags : unsigned int
			{
				kRead = 0x1, //!< Read access
				kWrite = 0x2, //!< Write access
				kBinary = 0x4 //!< Binary read
			};

			/**
			* @brief Opens the file for use with specified access flags
			* @param[in] path (const snuffbox::engine::String&) The path to load the file from
			* @param[in] flags (const unsigned int&) The access flags to open the file with
			* @return (snuffbox::engine::File*) The opened file, File::file_ will be nullptr if opening failed
			*/
			static File* Open(const engine::String& path, const unsigned int& flags);

			/**
			* @brief Closes the file and deallocates all used memory
			* @param[in] file (snuffbox::engine::File*) The file to close
			*/
			static void Close(File* file);

			/**
			* @return (const unsigned char*) The file's buffer as a binary format
			*/
			const unsigned char* Binary();

			/**
			* @return (const char*) The file's buffer as a string format
			*/
			const char* String();

			/**
			* @return (const snuffbox::engine::String&) The path this file originates from
			*/
			const engine::String& path() const;

			/**
			* @brief Default destructor, cleans up any memory in use
			*/
			~File();

		private:

			engine::String path_; //!< The path this file originates from
			FILE* file_; //!< The actual file handle

			unsigned char* buffer_; //!< The buffer to allocate the file data in
		};
	}
}