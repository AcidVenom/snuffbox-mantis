#pragma once

#include "../defines.h"
#include <logging_stream.h>

#include <connection/logging_server.h>

namespace snuffbox
{
	class Console;

	class Server : public snuffbox::LoggingServer
	{

	public:

		Server();

		Server(Console* console);

		void OnConnect() const override;

		void OnDisconnect() const override;

	private:

		Console* console_;
	};

	/**
	* @class snuffbox::ConsoleApp : public wxApp
	* @brief The wxWidgets application that will contain all the wx windows
	* @author Daniël Konings
	*/
	class ConsoleApp : public wxApp
	{
	public:

		/**
		* @brief Called when the wxApp is initialised
		* @return (bool) Was the initialisation a success?
		*/
		bool OnInit() override;

		/**
		* @brief Called when the wxApp is closed
		* @return (int) The exit code
		*/
		int OnExit() override;

	private:

		Console* console_; //!< The main console window of the application generated through a form
		LoggingStream stream_; //!< The logging stream
		Server server_; //!< The logging server
	};
}