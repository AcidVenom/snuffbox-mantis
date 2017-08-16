#include "shader_compiler.h"
#include "../utils/glslang_validator.h"

namespace snuffbox
{
	namespace compilers
	{
		//-----------------------------------------------------------------------------------------------
		const uint32_t ShaderCompiler::TYPE_ = 0x00485353;

		//-----------------------------------------------------------------------------------------------
		ShaderCompiler::ShaderCompiler(Allocation allocator, Deallocation deallocator) :
			Compiler(allocator, deallocator)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool ShaderCompiler::Compilation(const unsigned char* input, size_t size, size_t* out_size, const unsigned char* userdata)
		{
			if (userdata == nullptr)
			{
				return false;
			}

			const char* full_path = reinterpret_cast<const char*>(userdata);
			char type = GetShaderTypeFromExtension(full_path);

			if (type == -1)
			{
				return false;
			}

			unsigned char* bin = nullptr;
			size_t bin_size;

			GLSLangValidator validator;

			if (validator.Compile(reinterpret_cast<const char*>(input), size, full_path, static_cast<GLSLangValidator::ShaderType>(type), &bin_size, &bin) == false)
			{
				SetError(validator.GetError());
				return false;
			}

			const size_t header_size = sizeof(Compiler::FileHeader);
			data_ = Allocate(bin_size + header_size);

			FileHeader header;
			header.magic = FileHeader::MAGIC;
			header.file_type = TYPE_;
			header.file_size = bin_size;

			memcpy(data_, &header, header_size);
			memcpy(data_ + header_size, bin, bin_size);

			if (out_size != nullptr)
			{
				*out_size = bin_size;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool ShaderCompiler::Decompilation(const unsigned char* input, const unsigned char* userdata)
		{
			return true;
		}

		//-----------------------------------------------------------------------------------------------
		char ShaderCompiler::GetShaderTypeFromExtension(const char* path)
		{
			int len = static_cast<int>(strlen(path));

			if (len == 0)
			{
				return -1;
			}
			
			int last;
			for (last = len - 1; last >= 0; --last)
			{
				if (path[last] == '.')
				{
					break;
				}
			}

			const char* ext = &path[last];

			if (strcmp(ext, ".vs") == 0)
			{
				return static_cast<char>(GLSLangValidator::ShaderType::kVertex);
			}
			else if (strcmp(ext, ".ps") == 0)
			{
				return static_cast<char>(GLSLangValidator::ShaderType::kPixel);
			}
			else if (strcmp(ext, ".gs") == 0)
			{
				return static_cast<char>(GLSLangValidator::ShaderType::kGeometry);
			}

			return -1;
		}
	}
}