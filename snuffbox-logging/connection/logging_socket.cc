#include "logging_socket.h"
#include "../logging_stream.h"

#ifdef SNUFF_WIN32
#include "../win32/winsock_wrapper.h"
#endif

#include "logging_wrapper.h"

#include "logging_client.h"
#include "logging_server.h"

namespace snuffbox
{
	namespace logging
	{
		//-----------------------------------------------------------------------------------------------
		const unsigned int LoggingSocket::DISCONNECTED_SLEEP_ = 100;

		//-----------------------------------------------------------------------------------------------
		LoggingSocket::LoggingSocket() :
			socket_(-1),
			other_(-1),
			connected_(false),
			skip_(false)
		{
			time(&last_time_);
		}

		//-----------------------------------------------------------------------------------------------
		bool LoggingSocket::TimedOut(const unsigned int& timeout) const
		{
			time_t now;
			time(&now);

			if (static_cast<unsigned int>(difftime(now, last_time_)) >= timeout)
			{
				return true;
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool LoggingSocket::ReceivePacket(const int& socket, const int& expected_size, const bool& quit)
		{
			int bytes = 0;
			int result = -1;
			int err = 0;

			while (quit == false)
			{
				result = recv(socket, buffer_, expected_size, 0);
				err = errno;

				if ((result < 0 && (err == SNUFF_WOULD_BLOCK || err == EAGAIN)) && TimedOut() == false)
				{
					continue;
				}
				else if (result > 0)
				{
					time(&last_time_);
					return true;
				}

				break;
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		bool LoggingSocket::SendPacket(const int& socket, const char* buffer, const int& size, const bool& quit)
		{
			int bytes = 0;
			int result = -1;
			int err = 0;

			while (quit == false)
			{
				result = send(socket, buffer, size, 0);
				err = errno;

				if ((result < 0 && (err == SNUFF_WOULD_BLOCK || err == EAGAIN)) && TimedOut() == false)
				{
					continue;
				}
				else if (result > 0)
				{
					time(&last_time_);
					return true;
				}

				break;
			}

			return false;
		}

		//-----------------------------------------------------------------------------------------------
		std::thread& LoggingSocket::Execute(const std::function<void()>& func)
		{
			if (execution_thread_.joinable() == true)
			{
				execution_thread_.join();
			}

			execution_thread_ = std::thread(func);

			return execution_thread_;
		}

		//-----------------------------------------------------------------------------------------------
		void LoggingSocket::OnConnect(const bool& stream_quit) const
		{

		}

		//-----------------------------------------------------------------------------------------------
		void LoggingSocket::OnDisconnect(const bool& stream_quit) const
		{

		}

		//-----------------------------------------------------------------------------------------------
		LoggingSocket::~LoggingSocket()
		{
			if (execution_thread_.joinable() == true)
			{
				execution_thread_.join();
			}
		}
	}
}