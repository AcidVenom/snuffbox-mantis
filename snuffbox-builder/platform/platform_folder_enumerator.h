#pragma once

#ifdef SNUFF_WIN32
#include "../utils/win32_folder_enumerator.h"
#elif defined SNUFF_LINUX
#include "../utils/linux_folder_enumerator.h"
#endif

namespace snuffbox
{
	namespace builder
	{
#ifdef SNUFF_WIN32
		typedef Win32FolderEnumerator FolderEnumerator;
#else
		typedef LinuxFolderEnumerator FolderEnumerator;
#endif
	}
}
