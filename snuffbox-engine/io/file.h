#pragma once

#include <stdio.h>

#include "../core/eastl.h"
#include <time.h>

#include "../js./js_defines.h"

#ifdef SNUFF_WIN32
#define fopen(out, path, flags) fopen_s(&out, path, flags);
#define fread(out, size, file) fread_s(out, size, size, 1, file)
#else
#define fopen(out, path, flags) out = fopen(path, flags);
#define fread(out, size, file) size_t filesize_fread = fread(out, size, 1, file);
#endif

namespace snuffbox
{
	namespace engine
	{
		class Allocator;

		/**
		* @class snuffbox::engine::File : [JSObject]
		* @brief A helper class for cross-platform file reading/writing
		* @author Daniel Konings
		*/
		class File : JS_OBJECT
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
			* @param[in] null_terminated (bool) Should the buffer be null-terminated?
			*/
			void Read(bool null_terminated);

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
			* @param[in] flags (unsigned int) The access flags to open the file with
			* @param[in] relative (bool) Should the path be relative to the current 'src_directory' CVar?
			* @param[in] opened (snuffbox::engine::File*) If there's a file to be re-opened, use this file instead, default = nullptr
			* @return (snuffbox::engine::File*) The opened file, File::file_ will be nullptr if opening failed
			*/
			static File* Open(const engine::String& path, unsigned int flags, bool relative = false, File* opened = nullptr);

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
			* @brief Writes data into the opened file
			* @remarks The file will be emptied (removed and readded) first, before writing to it
			* @param[in] data (const unsigned char*) The data to write to the file
			* @param[in] size (size_t) The size of the data to write to the file
			*/
			bool Write(const unsigned char* data, size_t size);

			/**
			* @return (const snuffbox::engine::String&) The path this file originates from
			*/
			const engine::String& path() const;

			/**
			* @brief Default destructor, cleans up any memory in use
			*/
			~File();

		private:

			FILE* file_; //!< The actual file handle

			engine::String path_; //!< The path this file originates from
			bool relative_; //!< Was the file loaded relatively to the source directory?

			unsigned char* buffer_; //!< The buffer to allocate the file data in

		public:

			JS_NAME_TMPL(File);

			JS_FUNCTION_DECL(open);
			JS_FUNCTION_DECL(read);
			JS_FUNCTION_DECL(write);
			JS_FUNCTION_DECL(close);
		};
	}
}
