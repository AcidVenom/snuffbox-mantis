#pragma once

#include <type_traits>

namespace snuffbox
{
	namespace engine
	{
		/**
		* @brief Used to extend type information on the different services available
		*/
		enum ServiceIDs : int
		{
			kLogService, //!< The ID for the log service
			kCVarService, //!< The ID for the CVar service
			kContentService, //!< The ID for the content service
			kWindowService, //!< The ID for the window service
			kCount //!< The total number of different services
		};

		/**
		* @struct snuffbox::engine::ServiceBase
		* @brief The service base for every service to use, this is used in conjuction with the type checking
		* @author Daniel Konings
		*/
		struct ServiceBase
		{

		};

		/**
		* @struct snuffbox::engine::Service<int> : public snuffbox::engine::ServiceBase
		* @brief Used to inherit typing information to the different services
		* @author Daniel Konings
		*/
		template <int T>
		struct Service : public ServiceBase
		{
			static const int SERVICE_ID = T; //!< The service ID of this service class
		};

		/**
		* @struct snuffbox::engine::is_service<T>
		* @brief Used to check if a class is actually a service
		* @author Daniel Konings
		*/
		template <typename T>
		struct is_service
		{
			static const bool value = std::is_base_of<ServiceBase, T>::value;
		};
	}
}
