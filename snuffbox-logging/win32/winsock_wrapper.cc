#ifdef SNUFF_WIN32
#include "winsock_wrapper.h"

#include <assert.h>

namespace snuffbox
{
	namespace logging
	{
		//-----------------------------------------------------------------------------------------------
		WinSockWrapper::WinSockWrapper() :
			initialised_(false)
		{

		}

		//-----------------------------------------------------------------------------------------------
		char* WinSockWrapper::GetErrorMessage(int error)
		{
			LPSTR s = nullptr;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&s), 0, NULL);

			return s;
		}

		//-----------------------------------------------------------------------------------------------
		int WinSockWrapper::Initialise()
		{
			int result;

			result = WSAStartup(WINSOCK_VERSION, &data_);

			if (result == 0)
			{
				initialised_ = true;
			}
			return result;
		}

		//-----------------------------------------------------------------------------------------------
		int WinSockWrapper::CleanUp()
		{
			assert(initialised_ == true);

			if (initialised_ == true)
			{
				initialised_ = false;
				return WSACleanup();
			}

			return -1;
		}
	}
}
#endif