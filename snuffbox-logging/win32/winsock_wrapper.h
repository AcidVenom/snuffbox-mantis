#pragma once

#ifdef SNUFF_WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <string>

namespace snuffbox
{
	namespace logging
	{
		/**
		* @class snuffbox::logging::WinSockWrapper
		* @brief A wrapper class to initialise the WinSock library with
		* @author Dani�l Konings
		*/
		class WinSockWrapper
		{
		public:

			/**
			* @brief Default constructor
			*/
			WinSockWrapper();

			/**
			* @brief Converts an error code to a descriptive string
			* @param[in] error (const int&) The error code to convert
			* @return (char*) The error message as a string
			*/
			char* GetErrorMessage(const int& error);

			/**
			* @brief Initialises WinSock
			* @return (int) The error code, success = 0
			*/
			int Initialise();

			/**
			* @brief Cleans up WinSock
			* @return (int) The error code, success = 0
			*/
			int CleanUp();

		private:

			bool initialised_; //!< Has the WinSock wrapper been initialised?
			WSADATA data_; //!< The WinSock data that is filled in at initialisation
		};
	}
}
#endif