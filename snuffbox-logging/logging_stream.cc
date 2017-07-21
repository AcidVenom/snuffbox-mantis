#include "logging_stream.h"

#include "connection/logging_server.h"
#include "connection/logging_client.h"

#include <thread>

#ifdef SNUFF_WIN32
#include "win32/winsock_wrapper.h"
#endif

#include "connection/logging_wrapper.h"

namespace snuffbox
{
	namespace logging
	{
		//-----------------------------------------------------------------------------------------------
		const unsigned int LoggingStream::STARTUP_SLEEP_ = 1000;
		const unsigned int LoggingStream::WAIT_SLEEP_ = 8;
		const unsigned int LoggingStream::SHUTDOWN_SLEEP_ = 1000;

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

			if (is_server_ == false)
			{
#ifdef SNUFF_DEBUG
				printf("Waiting %ims for server..\n", STARTUP_SLEEP_);
#endif
				std::this_thread::sleep_for(std::chrono::milliseconds(STARTUP_SLEEP_));
			}
		}

		//-----------------------------------------------------------------------------------------------
		void LoggingStream::RunThread(LoggingSocket* socket, const int& port, const char* ip)
		{
			connection_thread_ = std::thread([=]()
			{
				LoggingSocket::ConnectionStatus status = LoggingSocket::ConnectionStatus::kWaiting;
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
						std::unique_lock<std::mutex> lock(connection_mutex_);

						if (is_server_ == true)
						{
							connection_cv_.notify_one();
						}

						status = socket->Update(should_quit_);

						switch (status)
						{
						case LoggingSocket::ConnectionStatus::kWaiting:
							if (is_server_ == false)
							{
								connection_cv_.notify_one();
							}
							break;
						case LoggingSocket::ConnectionStatus::kDisconnected:
							disconnect = true;
							break;
						}
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_SLEEP_));
				}

				socket->CloseSocket(should_quit_);
				connection_cv_.notify_all();
			});
		}

		//-----------------------------------------------------------------------------------------------
		void LoggingStream::Log(const console::LogSeverity& severity, const char* message, const int& size, const unsigned char* col_bg, const unsigned char* col_fg)
		{
			assert(is_server_ == false);

			if (socket_->connected_ == false)
			{
				return;
			}

			char buffer[SNUFF_LOG_BUFFERSIZE];

			int offset = 0;
			memset(buffer, static_cast<char>(severity), sizeof(char));
			memcpy(buffer + (++offset), message, size);
			memset(buffer + offset + size, '\0', sizeof(char));

			int extra_size = 1;

			if (severity == console::LogSeverity::kRGB && col_fg != nullptr && col_bg != nullptr)
			{
				memcpy(buffer + size + 1 + offset, col_fg, sizeof(unsigned char) * 3);
				offset += 3;
				memcpy(buffer + size + 1 + offset, col_bg, sizeof(unsigned char) * 3);

				extra_size += sizeof(unsigned char) * 6;
			}

			int s = size + 1 + extra_size;

			Send(Commands::kLog, socket_->socket_, buffer, s);
		}

		//-----------------------------------------------------------------------------------------------
		void LoggingStream::SendCommand(const Commands& cmd, const char* message, const int& size)
		{
			assert(is_server_ == true);

			if (socket_->connected_ == false)
			{
				return;
			}

			char buffer[SNUFF_LOG_BUFFERSIZE];

			int offset = 0;
			memset(buffer, static_cast<char>(cmd), sizeof(char));
			memcpy(buffer + (++offset), message, size);
			memset(buffer + offset + size, '\0', sizeof(char));

			int s = size + 2;

			Send(cmd, socket_->other_, buffer, s);
		}

		//-----------------------------------------------------------------------------------------------
		void LoggingStream::Close()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(SHUTDOWN_SLEEP_));
			should_quit_ = true;

			if (connection_thread_.joinable() == true)
			{
				connection_thread_.join();
			}

#ifdef SNUFF_WIN32
			win_sock_->CleanUp();
#endif
		}

		//-----------------------------------------------------------------------------------------------
		bool LoggingStream::Connected() const
		{
			return socket_ != nullptr && socket_->connected_ == true;
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
			error_handler_(std::to_string(error).c_str());
#endif
		}

		//-----------------------------------------------------------------------------------------------
		void LoggingStream::Send(const Commands& cmd, const int& other, const char* buffer, const int& size)
		{
			if (socket_->connected_ == false)
			{
				return;
			}

			std::unique_lock<std::mutex> lock(connection_mutex_);
			connection_cv_.wait(lock);

			if (socket_->connected_ == false)
			{
				return;
			}

			socket_->skip_ = true;

			Packet packet;
			packet.command = cmd;
			memcpy(packet.buffer, buffer, sizeof(Packet) - sizeof(Commands));

			bool connected = socket_->SendPacket(other, reinterpret_cast<char*>(&packet), sizeof(Packet), should_quit_);
		}
	}
}