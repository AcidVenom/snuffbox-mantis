#pragma once

#include "../defines.h"
#include <logging_stream.h>

#include <connection/logging_server.h>

namespace snuffbox
{
	class Console;

	/**
	* @class snuffbox::ConsoleServer : public snuffbox::LogginServer
	* @brief The console server that we can log message from the logging stream with
	* @author Daniël Konings
	*/
	class ConsoleServer : public snuffbox::LoggingServer
	{

	public:

		/**
		* @brief Default constructor
		*/
		ConsoleServer();

		/**
		* @brief Construct by providing the console that we'll append to
		* @param[in] console (snuffbox::Console*) The console
		*/ 
		ConsoleServer(Console* console);

		/**
		* @see snuffbox::LoggingServer::OnConnect
		*/
		void OnConnect(const bool& stream_quit) const override;

		/**
		* @see snuffbox::LoggingServer::OnDisconnect
		*/
		void OnDisconnect(const bool& stream_quit) const override;

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
		ConsoleServer server_; //!< The logging server
	};
}