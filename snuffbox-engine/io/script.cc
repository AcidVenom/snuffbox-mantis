#include "script.h"
#include "file.h"

#include "../services/log_service.h"

#ifdef SNUFF_JAVASCRIPT
#include "../js/js_state_wrapper.h"
#endif

#include <snuffbox-compilers/compilers/script_compiler.h>

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		bool Script::Load(File* file)
		{
			LogService& log = Services::Get<LogService>();

#ifdef SNUFF_JAVASCRIPT
			const unsigned char* buffer = file->Binary();
			if (buffer == nullptr)
			{
				return false;
			}

			compilers::ScriptCompiler c(
				[](size_t size) { return Memory::default_allocator().Malloc(size); },
				[](void* ptr) { Memory::default_allocator().Free(ptr); });

			const unsigned char* output;
			bool decompiled = c.Decompile(buffer, &output);

			if (decompiled == false)
			{
				log.Log(console::LogSeverity::kError, "Could not decompile '{0}'", file->path());
				return false;
			}

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			String error;
			bool success = wrapper->Run(reinterpret_cast<const char*>(output), file->path(), nullptr, &error);

			if (success == false)
			{
				log.Log(console::LogSeverity::kError, error);
				return false;
			}

			return true;
#else
			return false;
#endif
		}

		//-----------------------------------------------------------------------------------------------
		bool Script::Reload(File* file)
		{
			return Load(file);
		}
	}
}