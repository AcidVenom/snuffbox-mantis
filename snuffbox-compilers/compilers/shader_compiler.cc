#include "shader_compiler.h"
#include "../utils/glslang_validator.h"

namespace snuffbox
{
	namespace compilers
	{
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
			const size_t shader_header_size = sizeof(ShaderCompiler::Header);

			size_t file_size = bin_size + sizeof(ShaderCompiler::Header);

			data_ = Allocate(file_size + header_size);

			FileHeader header = CreateFileHeader(file_size, Magic::kShader);

			ShaderCompiler::Header shader_header;
			shader_header.size = bin_size;
			shader_header.type = type;

			memcpy(data_, &header, header_size);
			memcpy(data_ + header_size, &shader_header, shader_header_size);
			memcpy(data_ + header_size + shader_header_size, bin, bin_size);

			if (out_size != nullptr)
			{
				*out_size = file_size + header_size;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		bool ShaderCompiler::Decompilation(const unsigned char* input, size_t* out_size, const unsigned char* userdata)
		{
			Compiler::FileHeader header;
			
			if (GetFileHeader(input, Magic::kShader, &header) == false)
			{
				return false;
			}

			const size_t s = header.file_size;

#ifdef SNUFF_USE_VULKAN
			data_ = Allocate(s);
			memcpy(data_, input + sizeof(FileHeader), s);
#else
			SetError("No implementation provided for shaders for this type of renderer");
			return false;
#endif

			if (out_size != nullptr)
			{
				*out_size = s;
			}

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