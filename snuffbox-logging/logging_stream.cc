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
		is_server_(false),
		socket_(nullptr),
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
		is_server_ = true;
		socket_ = server;
		Start(server, port, "");
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::Open(LoggingClient* client, const int& port, const char* ip)
	{
		is_server_ = false;
		socket_ = client;
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

		RunThread(socket, port, ip);
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingStream::RunThread(LoggingSocket* socket, const int& port, const char* ip)
	{
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
					connection_mutex_.lock();
					status = socket->Update(should_quit_);
					connection_mutex_.unlock();
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
	void LoggingStream::Log(const LogSeverity& severity, const char* message, const int& size)
	{
		assert(is_server_ == false);

		if (socket_->connected_ == false)
		{
			return;
		}

		std::lock_guard<std::mutex> lock(connection_mutex_);

		bool connected = true;

		PacketHeader header;
		header.command = Commands::kLog;
		header.severity = severity;
		header.size = size;

		connected = socket_->Send(socket_->socket_, &header, should_quit_);
		connected = connected == false ? false : socket_->Receive(socket_->socket_, &header, should_quit_);

		if (connected == true && header.command == Commands::kAccept)
		{
			connected = socket_->SendPacket(socket_->socket_, message, size, should_quit_);
			connected = connected == false ? false : socket_->Receive(socket_->socket_, &header, should_quit_);
		}
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