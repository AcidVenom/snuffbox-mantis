#include "logging_stream.h"

#include "connection/logging_server.h"
#include "connection/logging_client.h"

#include <thread>

#ifdef SNUFF_WIN32
#include "win32/winsock_wrapper.h"
#endif

#include "connection/wrapper.h"

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	LoggingStream::LoggingStream() :
#ifdef SNUFF_WIN32
		win_sock_(nullptr),
#endif
		should_quit_(false),
		error_handler_(nullptr)
	{
		static WinSockWrapper wrapper;
		win_sock_ = &wrapper;

		win_sock_->Initialise();
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::Open(LoggingServer* server, const int& port)
	{
		int err = 0;
		if ((err = server->OpenSocket(port)) != 0)
		{
			LogError(err);
			return;
		}

		connection_thread_ = std::thread([=]()
		{
			while (should_quit_ == false)
			{
				if (server->Connect(should_quit_) == 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(SNUFF_SLEEP_WAITING));
				}
			}

			server->CloseSocket();
		});
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::Open(LoggingClient* client, const int& port, const char* ip)
	{
		int err = 0;
		if ((err = client->OpenSocket()) != 0)
		{
			LogError(err);
			return;
		}

		connection_thread_ = std::thread([=]()
		{
			while (should_quit_ == false)
			{
				if (client->Connect(port, ip, should_quit_) == 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(SNUFF_SLEEP_WAITING));
				}
			}

			client->CloseSocket();
		});
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::Close()
	{
		should_quit_ = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(SNUFF_SLEEP_SHUTDOWN));

		if (connection_thread_.joinable() == true)
		{
			connection_thread_.join();
		}

		win_sock_->CleanUp();
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::SetErrorHandler(void(*func)(const char*))
	{
		error_handler_ = func;
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::LogError(const int& error) const
	{
		if (error_handler_ == nullptr)
		{
			return;
		}

#ifdef SNUFF_WIN32
		char* buff = win_sock_->GetErrorMessage(error);
		error_handler_(buff);
		LocalFree(buff);
#else
		error_handler_(error);
#endif
	}
}