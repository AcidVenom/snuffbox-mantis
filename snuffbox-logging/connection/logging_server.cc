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
	void LoggingServer::CloseSocket()
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
			OnDisconnect(true);
		}
	}

	//-----------------------------------------------------------------------------------------------
	LoggingSocket::ConnectionStatus LoggingServer::Update(const bool& quit)
	{
		bool connected = true;
		LoggingStream::PacketHeader header;

		if (last_message_ == LoggingStream::Commands::kWaiting)
		{
			connected = ReceivePacket(other_, sizeof(char), quit);
			if (connected == true)
			{
				header.command = LoggingStream::Commands::kWaiting;
				header.size = 0;
				connected = SendPacket(other_, reinterpret_cast<char*>(&header), sizeof(LoggingStream::PacketHeader), quit);

				return ConnectionStatus::kWaiting;
			}
		}

		return ConnectionStatus::kDisconnected;
	}
}