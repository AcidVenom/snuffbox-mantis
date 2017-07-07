#include "logging_server.h"
#include "logging_stream.h"

#ifdef SNUFF_WIN32
#include "win32/winsock_wrapper.h"
#endif

#include "wrapper.h"

#include <thread>

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	LoggingServer::LoggingServer()
	{

	}

	//-----------------------------------------------------------------------------------------------
	int LoggingServer::OpenSocket(const int& port)
	{
		socket_ = static_cast<int>(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

		if (socket_ < 0)
		{
			return errno;
		}

		sockaddr_in server_addr;
		memset(&server_addr, 0, sizeof(sockaddr_in));

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		int error = 0;

		if ((error = bind(socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(sockaddr_in))) < 0)
		{
			return errno;
		}

		listen(socket_, 1);

		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	int LoggingServer::Connect(const int& port, const char* ip, const bool& quit)
	{
		if (quit == true)
		{
			return -1;
		}

		assert(socket_ >= 0);

		if (socket_ < 0 || connected_ == true)
		{
			return 0;
		}

		other_ = -1;

		set_blocking_socket(socket_, false);
		while (other_ <= 0)
		{
			if (quit == true)
			{
				return -1;
			}

			other_ = static_cast<int>(accept(socket_, nullptr, nullptr));
			std::this_thread::sleep_for(std::chrono::milliseconds(SNUFF_SLEEP_DISCONNECTED));
		}

		connected_ = true;
		OnConnect(quit);
		time(&last_time_);

		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingServer::CloseSocket(const bool& quit)
	{
		if (socket_ > 0)
		{
			closesocket(socket_);
		}

		if (other_ > 0)
		{
			closesocket(other_);
		}

		if (connected_ == true)
		{
			connected_ = false;
			OnDisconnect(quit);
		}
	}

	//-----------------------------------------------------------------------------------------------
	bool LoggingServer::SendAccept(const bool& quit)
	{
		LoggingStream::PacketHeader header;
		header.command = LoggingStream::Commands::kAccept;
		header.severity = LogSeverity::kCount;
		header.size = -1;

		return Send(other_, &header, quit);
	}

	//-----------------------------------------------------------------------------------------------
	LoggingSocket::ConnectionStatus LoggingServer::Update(const bool& quit)
	{
		bool connected = true;
		LoggingStream::PacketHeader header;

		if (last_message_ < LoggingStream::Commands::kLog || last_message_ == LoggingStream::Commands::kProcessing)
		{
			connected = Receive(other_, &header, quit);
			if (connected == true)
			{
				last_message_ = header.command;

				if (last_message_ == LoggingStream::Commands::kLog)
				{
					expected_ = header.size;
					severity_ = header.severity;

					connected = SendAccept(quit);

					if (connected == false)
					{
						return ConnectionStatus::kDisconnected;
					}

					return ConnectionStatus::kBusy;
				}

				SendWait(other_, quit);

				return ConnectionStatus::kWaiting;
			}
		}
		else
		{
			connected = ReceivePacket(other_, expected_, quit);

			if (connected == true)
			{
				if (severity_ == LogSeverity::kRGB)
				{
					OnLog(severity_, buffer_,
						reinterpret_cast<unsigned char*>(buffer_ + expected_ - 6),
						reinterpret_cast<unsigned char*>(buffer_ + expected_ - 3));
				}
				else
				{
					OnLog(severity_, buffer_);
				}

				connected = SendWait(other_, quit);
				last_message_ = LoggingStream::Commands::kWaiting;

				return ConnectionStatus::kWaiting;
			}
		}

		return ConnectionStatus::kDisconnected;
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingServer::OnLog(const LogSeverity& severity, const char* message, const unsigned char* col_fg, const unsigned char* col_bg)
	{

	}
}