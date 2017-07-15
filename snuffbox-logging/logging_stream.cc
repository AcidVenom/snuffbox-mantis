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
		const unsigned int LoggingStream::STARTUP_SLEEP_ = 1500;
		const unsigned int LoggingStream::WAIT_SLEEP_ = 1;
		const unsigned int LoggingStream::SHUTDOWN_SLEEP_ = 500;

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
							std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_SLEEP_));
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
			should_quit_ = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(SHUTDOWN_SLEEP_));

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
			std::lock_guard<std::recursive_mutex> lock(connection_mutex_);

			if (socket_->connected_ == false)
			{
				return;
			}

			PacketHeader header;
			header.command = cmd;
			header.size = size;

			bool connected = socket_->Send(other, &header, should_quit_);
			connected = connected == false ? false : socket_->Receive(other, &header, should_quit_);

			if (connected == true && (header.command == Commands::kWaiting || header.command == Commands::kAccept))
			{
				connected = socket_->SendPacket(other, buffer, size, should_quit_);
				socket_->Receive(other, &header, should_quit_);
			}
		}
	}
}