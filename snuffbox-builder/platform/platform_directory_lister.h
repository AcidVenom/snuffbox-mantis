#pragma once

#ifdef SNUFF_WIN32
#include "../utils/win32_directory_lister.h"
#elif defined SNUFF_LINUX
#include "../utils/linux_directory_lister.h"
#endif

namespace snuffbox
{
	namespace builder
	{
#ifdef SNUFF_WIN32
		typedef Win32DirectoryLister DirectoryLister;
#elif defined SNUFF_LINUX
		typedef LinuxDirectoryLister DirectoryLister;
#endif
	}
}
