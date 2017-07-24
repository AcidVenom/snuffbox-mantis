#include "app.h"
#include <cmath>
#include <snuffbox-engine/core/eastl.h>

#include <snuffbox-engine/services/log_service.h>
#include <snuffbox-engine/services/cvar_service.h>

#include <snuffbox-engine/io/file.h>
#include <snuffbox-compilers/compilers/script_compiler.h>

namespace snuffbox
{
	namespace test
	{
		//-----------------------------------------------------------------------------------------------
		App::App()
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		void App::OnInit()
		{
			/*compilers::Compiler::Allocation alloc = [](size_t size) { return engine::Memory::default_allocator().Malloc(size); };
			compilers::Compiler::Deallocation dealloc = [](void* ptr) { engine::Memory::default_allocator().Free(ptr); };
			compilers::ScriptCompiler c(alloc, dealloc);

			engine::File* f1 = engine::File::Open("F:/Programming/snuffbox-test/main.js", 
				engine::File::AccessFlags::kRead | engine::File::AccessFlags::kBinary);

			engine::File* f2 = engine::File::Open("F:/Programming/snuffbox-test/converted.js",
				engine::File::AccessFlags::kWrite | engine::File::AccessFlags::kBinary);
			
			engine::String str = f1->String();
			size_t new_size;
			const unsigned char* buffer;
			if (c.Compile(reinterpret_cast<const unsigned char*>(str.c_str()), str.size() + 1, &new_size, &buffer) == true)
			{
				if (f2->Write(buffer, new_size) == false)
				{
					return;
				}
			}

			engine::File::Close(f1);
			engine::File::Close(f2);

			f2 = engine::File::Open("F:/Programming/snuffbox-test/converted.js", engine::File::AccessFlags::kRead | engine::File::AccessFlags::kBinary);
			const unsigned char* compiled = f2->Binary();

			if (c.Decompile(compiled, &buffer) == true)
			{
				printf("Decompiled: %s\n", buffer);
			}

			engine::File::Close(f2);*/
		}

		//-----------------------------------------------------------------------------------------------
		void App::OnUpdate(float dt)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void App::OnShutdown()
		{

		}
	}
}
