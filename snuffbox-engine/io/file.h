#pragma once

#include <stdio.h>

#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		class Allocator;

		class File
		{

			friend class Allocator;

		protected:

			/**
			* @brief Default constructor
			*/
			File();

			File(const File& other) = default;

			/**
			* @brief Reads all the file data into this class' buffer
			*/
			void Read();

		public:

			enum AccessFlags : unsigned int
			{
				kRead = 0x1,
				kWrite = 0x2,
				kBinary = 0x4
			};

			static File* Open(const engine::String& path, const unsigned int& flags);
			static void Close(File* file);

			const unsigned char* Binary();
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