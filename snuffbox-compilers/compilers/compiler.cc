#include "compiler.h"

#include <memory>
#include <assert.h>

namespace snuffbox
{
	namespace compilers
	{
		//-----------------------------------------------------------------------------------------------
		const uint32_t Compiler::FileHeader::MAGIC = 0x46554E53;

		//-----------------------------------------------------------------------------------------------
		Compiler::Compiler(Allocation allocation, Deallocation deallocation) :
			allocator_(allocation),
			deallocator_(deallocation),
			data_(nullptr)
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		bool Compiler::Compile(const unsigned char* input, size_t size, size_t* out_size, const unsigned char** output, const unsigned char* userdata)
		{
			if (data_ != nullptr)
			{
				Deallocate(data_);
			}

			bool success = Compilation(input, size, out_size, userdata);

			if (output != nullptr)
			{
				*output = success == true ? data_ : nullptr;
			}

			return success;
		}

		//-----------------------------------------------------------------------------------------------
		bool Compiler::Decompile(const unsigned char* input, const unsigned char** output, const unsigned char* userdata)
		{
			if (data_ != nullptr)
			{
				Deallocate(data_);
			}

			bool success = Decompilation(input, userdata);

			if (output != nullptr)
			{
				*output = success == true ? data_ : nullptr;
			}

			return success;
		}

		//-----------------------------------------------------------------------------------------------
		Compiler::~Compiler()
		{
			if (data_ == nullptr)
			{
				return;
			}

			Deallocate(data_);
		}

		//-----------------------------------------------------------------------------------------------
		unsigned char* Compiler::Allocate(size_t size)
		{
			assert(data_ == nullptr);

			if (allocator_ == nullptr)
			{
				return static_cast<unsigned char*>(malloc(size));
			}

			return static_cast<unsigned char*>(allocator_(size));
		}

		//-----------------------------------------------------------------------------------------------
		void Compiler::Deallocate(void* ptr)
		{
			if (deallocator_ == nullptr)
			{
				free(ptr);
				data_ = nullptr;
				return;
			}

			deallocator_(ptr);
			data_ = nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		bool Compiler::GetFileHeader(const unsigned char* data, FileHeader* out)
		{
			const FileHeader* header = reinterpret_cast<const FileHeader*>(data);
			if (header->magic != FileHeader::MAGIC)
			{
				return false;
			}

			if (out != nullptr)
			{
				*out = *header;
			}

			return true;
		}
	}
}