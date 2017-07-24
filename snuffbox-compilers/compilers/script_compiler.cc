#include "script_compiler.h"
#include "../utils/rc4.h"

#include <memory>

namespace snuffbox
{
	namespace compilers
	{
		//-----------------------------------------------------------------------------------------------
		const uint32_t ScriptCompiler::TYPE_ = 0x00534A53;

		//-----------------------------------------------------------------------------------------------
		ScriptCompiler::ScriptCompiler(Compiler::Allocation allocator, Compiler::Deallocation deallocator) :
			Compiler(allocator, deallocator)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool ScriptCompiler::Compilation(const unsigned char* input, size_t size, size_t* out_size)
		{
			const size_t header_size = sizeof(Compiler::FileHeader);
			RC4 rc4;
			data_ = Allocate(size + header_size);

			if (data_ == nullptr)
			{
				return false;
			}

			FileHeader header;
			header.magic = FileHeader::MAGIC;
			header.file_type = TYPE_;
			header.file_size = size;

			memcpy(data_, &header, header_size);
			memcpy(data_ + header_size, input, size);

			rc4.Encrypt(reinterpret_cast<char*>(data_) + header_size, SNUFF_ENCRYPTION_KEY);

			if (out_size != nullptr)
			{
				*out_size = header.file_size + header_size;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool ScriptCompiler::Decompilation(const unsigned char* input)
		{
			FileHeader header;
			if (GetFileHeader(input, &header) == false)
			{
				return false;
			}

			if (header.magic != FileHeader::MAGIC || header.file_type != TYPE_)
			{
				return false;
			}

			RC4 rc4;

			const size_t s = header.file_size;
			data_ = Allocate(s);

			memcpy(data_, input + sizeof(FileHeader), s);

			rc4.Decrypt(reinterpret_cast<char*>(data_), SNUFF_ENCRYPTION_KEY);

			return true;
		}
	}
}