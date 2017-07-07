
#include "logging_socket.h"

namespace snuffbox
{
	/**
	* @class snuffbox::LoggingClient : public snuffbox::LoggingSocket
	* @brief The client sided socket to setup a logging stream
	* @author Daniël Konings
	*/
	class LoggingClient : public LoggingSocket
	{

	public:

		/**
		* @brief Default constructor
		*/
		LoggingClient();

	protected:

		/**
		* @see snuffbox::LoggingSocket::OpenSocket
		*/
		int OpenSocket(const int& port) override;

		/**
		* @see snuffbox::LoggingSocket::Connect
		*/
		int Connect(const int& port, const char* ip, const bool& quit) override;

		/**
		* @see snuffbox::LoggingSocket::CloseSocket
		*/
		void CloseSocket(const bool& quit) override;

		/**
		* @see snuffbox::LoggingSocket::Update
		*/
		ConnectionStatus Update(const bool& quit) override;
	};
}