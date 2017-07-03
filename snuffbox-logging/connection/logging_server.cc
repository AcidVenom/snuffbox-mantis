#include "logging_server.h"

#ifdef SNUFF_WIN32
#include "win32/winsock_wrapper.h"
#endif

#include "wrapper.h"

#include <thread>

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	LoggingServer::LoggingServer() :
		socket_(-1),
		client_(-1),
		connected_(false)
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
	void LoggingServer::Connect(const bool& quit)
	{
		assert(socket_ >= 0);

		if (socket_ < 0 || connected_ == true)
		{
			return;
		}

		client_ = -1;

		set_blocking_socket(socket_, false);
		while (client_ <= 0)
		{
			if (quit == true)
			{
				return;
			}

			client_ = static_cast<int>(accept(socket_, nullptr, nullptr));
			std::this_thread::sleep_for(std::chrono::milliseconds(SNUFF_SLEEP_DISCONNECTED));
		}

		connected_ = true;
		OnConnect();
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingServer::CloseSocket()
	{
		if (socket_ > 0)
		{
			closesocket(socket_);
		}

		if (client_ > 0)
		{
			closesocket(client_);
		}

		if (connected_ == true)
		{
			connected_ = false;
			OnDisconnect();
		}
	}

	//-----------------------------------------------------------------------------------------------
	void LoggingServer::OnConnect() const
	{

	}

	//-----------------------------------------------------------------------------------------------
	void LoggingServer::OnDisconnect() const
	{

	}
}