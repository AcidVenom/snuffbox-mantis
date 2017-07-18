#include "logging_server.h"
#include "../logging_stream.h"

#ifdef SNUFF_WIN32
#include "../win32/winsock_wrapper.h"
#endif

#include "logging_wrapper.h"

#include <thread>

namespace snuffbox
{
	namespace logging
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
				std::this_thread::sleep_for(std::chrono::milliseconds(DISCONNECTED_SLEEP_));
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
		LoggingSocket::ConnectionStatus LoggingServer::Update(const bool& quit)
		{
			const int size = sizeof(LoggingStream::Packet);
			bool connected = ReceivePacket(other_, size, quit);

			LoggingStream::Packet* packet = reinterpret_cast<LoggingStream::Packet*>(buffer_);
			if (packet->command == LoggingStream::Commands::kLog)
			{
				int len = static_cast<int>(strlen(buffer_ + 1));
				OnLog(*reinterpret_cast<console::LogSeverity*>(packet->buffer),
					packet->buffer + 1,
					reinterpret_cast<unsigned char*>(packet->buffer + len + 1),
					reinterpret_cast<unsigned char*>(packet->buffer + len + 4));
			}

			if (skip_ == false)
			{
				buffer_[0] = LoggingStream::Commands::kWaiting;
				connected == connected == false ? false : SendPacket(other_, buffer_, size, quit);
			}

			skip_ = false;

			return connected == true ? ConnectionStatus::kWaiting : ConnectionStatus::kDisconnected;
		}

		//-----------------------------------------------------------------------------------------------
		void LoggingServer::OnLog(const console::LogSeverity& severity, const char* message, const unsigned char* col_fg, const unsigned char* col_bg)
		{

		}
	}
}