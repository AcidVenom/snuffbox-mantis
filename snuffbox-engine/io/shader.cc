#include "shader.h"
#include "file.h"

#include "../services/log_service.h"
#include <snuffbox-compilers/compilers/shader_compiler.h>

#include "content_manager.h"
#include <snuffbox-graphics/platform/platform_renderer.h>

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Shader::Shader() :
			blob_(nullptr)
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool Shader::Load(File* file, ContentManager* cm)
		{
			LogService& log = Services::Get<LogService>();

			compilers::ShaderCompiler c(
				[](size_t size) { return Memory::default_allocator().Malloc(size); },
				[](void* ptr) { Memory::default_allocator().Free(ptr); });

			const unsigned char* buffer = file->Binary();
			const unsigned char* output;
			size_t size;

			bool decompiled = c.Decompile(buffer, &output, &size, nullptr);

			if (decompiled == false)
			{
				log.Log(console::LogSeverity::kError, "Could not decompile shader '{0}'\n\t{1}", file->path(), c.GetError());
				return false;
			}

			compilers::ShaderCompiler::Header h = *reinterpret_cast<const compilers::ShaderCompiler::Header*>(output);

			return cm->renderer()->CreateShader(output + sizeof(compilers::ShaderCompiler::Header), h.size, h.type, &blob_);
		}

		//-----------------------------------------------------------------------------------------------
		bool Shader::Reload(File* file, ContentManager* cm)
		{
			Release(cm);
			return Load(file, cm);
		}

		//-----------------------------------------------------------------------------------------------
		void Shader::Unload(ContentManager* cm)
		{
			Release(cm);
		}

		//-----------------------------------------------------------------------------------------------
		void Shader::Release(ContentManager* cm)
		{
			if (blob_ != nullptr)
			{
				cm->renderer()->ReleaseShader(blob_);
				blob_ = nullptr;
			}
		}
	}
}