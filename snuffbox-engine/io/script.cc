#include "script.h"
#include "file.h"

#include "../services/log_service.h"

#ifdef SNUFF_JAVASCRIPT
#include "../js/js_state_wrapper.h"
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		bool Script::Load(File* file)
		{
#ifdef SNUFF_JAVASCRIPT
			const char* buffer = file->String();
			if (buffer == nullptr)
			{
				return false;
			}

			JSStateWrapper* wrapper = JSStateWrapper::Instance();

			wrapper->isolate()->Enter();
			bool result = wrapper->Run(buffer, file->path(), false);
			wrapper->isolate()->Exit();

			return result;
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