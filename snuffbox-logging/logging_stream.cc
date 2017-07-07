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
#ifdef SNUFF_WIN32
		static WinSockWrapper wrapper;
		win_sock_ = &wrapper;

		win_sock_->Initialise();
#endif
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::Open(LoggingServer* server, const int& port)
	{
		Start(server, port, "");
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::Open(LoggingClient* client, const int& port, const char* ip)
	{
		Start(client, port, ip);
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::Start(LoggingSocket* socket, const int& port, const char* ip)
	{
		int err = 0;

		if ((err = socket->OpenSocket(port)) != 0)
		{
			LogError(err);
			return;
		}

		connection_thread_ = std::thread([=]()
		{
			LoggingSocket::ConnectionStatus status = LoggingSocket::ConnectionStatus::kWaiting;
			socket->last_message_ = Commands::kWaiting;
			bool disconnect = false;

			while (should_quit_ == false)
			{
				if (disconnect == true)
				{
					socket->CloseSocket(should_quit_);
					socket->OpenSocket(port);
					disconnect = false;
				}

				if (socket->Connect(port, ip, should_quit_) == 0)
				{
					status = socket->Update(should_quit_);
					switch (status)
					{
					case LoggingSocket::ConnectionStatus::kWaiting:
						std::this_thread::sleep_for(std::chrono::milliseconds(SNUFF_SLEEP_WAITING));
						break;
					case LoggingSocket::ConnectionStatus::kDisconnected:
						disconnect = true;
						break;
					}
				}
			}

			socket->CloseSocket(should_quit_);
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

#ifdef SNUFF_WIN32
		win_sock_->CleanUp();
#endif
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