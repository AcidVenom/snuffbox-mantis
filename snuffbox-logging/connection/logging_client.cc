#include "logging_client.h"
#include "logging_stream.h"

#ifdef SNUFF_WIN32
#include "win32/winsock_wrapper.h"
#endif

#include "wrapper.h"

#include <thread>

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	LoggingClient::LoggingClient()
	{

	}

	//-----------------------------------------------------------------------------------------------
	int LoggingClient::OpenSocket(const int& port)
	{
		socket_ = static_cast<int>(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

		if (socket_ < 0)
		{
			return errno;
		}

		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	int LoggingClient::Connect(const int& port, const char* ip, const bool& quit)
	{
		if (quit == true)
		{
			return -1;
		}

		assert(socket_ >= 0);

		if (connected_ == true)
		{
			return 0;
		}

		int result = -1;

		sockaddr_in server;
		memset(&server, 0, sizeof(sockaddr_in));

		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		if ((result = inet_pton(AF_INET, ip, &server.sin_addr.s_addr)) < 0)
		{
			return errno;
		}

		int err = 0;

		set_blocking_socket(socket_, false);
		while (result != 0)
		{
			if (quit == true)
			{
				return -1;
			}

			result = connect(socket_, reinterpret_cast<const sockaddr*>(&server), sizeof(sockaddr_in));

			err = errno;

			if (err != SNUFF_IS_CONNECTED)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(SNUFF_SLEEP_DISCONNECTED));
				continue;
			}

			break;
		}

		connected_ = true;
		OnConnect(quit);
		time(&last_time_);

		return 0;
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingClient::CloseSocket()
	{
		if (socket_ > 0)
		{
			closesocket(socket_);
		}

		if (connected_ == true)
		{
			connected_ = false;
			OnDisconnect(true);
		}
	}

	//-----------------------------------------------------------------------------------------------
	LoggingSocket::ConnectionStatus LoggingClient::Update(const bool& quit)
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