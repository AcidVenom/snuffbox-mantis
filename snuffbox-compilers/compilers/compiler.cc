#include "compiler.h"

#include <memory>
#include <assert.h>

namespace snuffbox
{
	namespace compilers
	{
		//-----------------------------------------------------------------------------------------------
		Compiler::Compiler(Allocation allocation, Deallocation deallocation) :
			allocator_(allocation),
			deallocator_(deallocation),
			data_(nullptr),
			error_message_(nullptr)
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
		bool Compiler::Decompile(const unsigned char* input, const unsigned char** output, size_t* out_size, const unsigned char* userdata)
		{
			if (data_ != nullptr)
			{
				Deallocate(data_);
			}

			bool success = Decompilation(input, out_size, userdata);

			if (output != nullptr)
			{
				*output = success == true ? data_ : nullptr;
			}

			return success;
		}

		//-----------------------------------------------------------------------------------------------
		const char* Compiler::GetError() const
		{
			return error_message_;
		}

		//-----------------------------------------------------------------------------------------------
		void Compiler::SetError(const char* error)
		{
			if (error_message_ != nullptr)
			{
				Deallocate(error_message_);
				error_message_ = nullptr;
			}

			size_t s = strlen(error) + 1;
			error_message_ = reinterpret_cast<char*>(Allocate(s));

			size_t null_terminated = s - 1;
			memcpy(error_message_, error, null_terminated);
			memset(error_message_ + null_terminated, '\0', sizeof(char));
		}

		//-----------------------------------------------------------------------------------------------
		Compiler::~Compiler()
		{
			if (error_message_ != nullptr)
			{
				Deallocate(error_message_);
				error_message_ = nullptr;
			}

			if (data_ != nullptr)
			{
				Deallocate(data_);
				data_ = nullptr;
			}
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
		bool Compiler::GetFileHeader(const unsigned char* data, Magic file_type, FileHeader* out)
		{
			const FileHeader* header = reinterpret_cast<const FileHeader*>(data);
			if (header->magic != Magic::kSnuffboxFile || header->file_type != file_type)
			{
				SetError("Unexpected file type");
				return false;
			}

			if (out != nullptr)
			{
				*out = *header;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		Compiler::FileHeader Compiler::CreateFileHeader(size_t file_size, Magic file_type)
		{
			FileHeader header;
			header.magic = Magic::kSnuffboxFile;
			header.file_type = file_type;
			header.file_size = file_size;

			return header;
		}
	}
}