#include "script_compiler.h"
#include "../utils/rc4.h"

#include <memory>
#include <string.h>

namespace snuffbox
{
	namespace compilers
	{
		//-----------------------------------------------------------------------------------------------
		ScriptCompiler::ScriptCompiler(Compiler::Allocation allocator, Compiler::Deallocation deallocator) :
			Compiler(allocator, deallocator)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool ScriptCompiler::Compilation(const unsigned char* input, size_t size, size_t* out_size, const unsigned char* userdata)
		{
			const size_t header_size = sizeof(Compiler::FileHeader);
			RC4 rc4;
			data_ = Allocate(size + header_size);

			if (data_ == nullptr)
			{
				SetError("Could not allocate data to store the encrypted script");
				return false;
			}

			FileHeader header = CreateFileHeader(size, Magic::kScript);

			memcpy(data_, &header, header_size);
			memcpy(data_ + header_size, input, size);

			rc4.Encrypt(reinterpret_cast<char*>(data_) + header_size, size, SNUFF_ENCRYPTION_KEY);

			if (out_size != nullptr)
			{
				*out_size = header.file_size + header_size;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool ScriptCompiler::Decompilation(const unsigned char* input, size_t* out_size, const unsigned char* userdata)
		{
			FileHeader header;
			if (GetFileHeader(input, Magic::kScript, &header) == false)
			{
				return false;
			}

			RC4 rc4;

			const size_t s = header.file_size;
			data_ = Allocate(s + 1);

			memcpy(data_, input + sizeof(FileHeader), s);

			rc4.Decrypt(reinterpret_cast<char*>(data_), s, SNUFF_ENCRYPTION_KEY);

			memset(data_ + s, '\0', sizeof(unsigned char));

			return true;
		}
	}
}
