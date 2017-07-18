#pragma once

#include "service.h"
#include <assert.h>
#include <mutex>

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Services
		* @brief Used to locate the different services that have been initialised
		* @author Daniël Konings
		*/
		class Services
		{

		public:

			/**
			* @brief Used to provide a service to the service locator
			* @param[in] service (T*) The service to provide
			* @remarks The typing information will be retrieved from 'T', if 'T' is not a service, this function will static_assert
			*/
			template <typename T>
			static void Provide(T* service);

			/**
			* @brief Used to retrieve a service from the service locator
			* @return (T&) The retrieved service based on 'T's service ID
			* @remarks The typing information will be retrieved from 'T', if 'T' is not a service, this function will static_assert
			*/
			template <typename T>
			static T& Get();

			/**
			* @brief Used to remove a service from the service locator, this will set it back to nullptr and the null-service will be used
			* @remarks The typing information will be retrieved from 'T', if 'T' is not a service, this function will static_assert
			*/
			template <typename T>
			static void Remove();

		private:

			static void* services_[ServiceIDs::kCount]; //!< The list of current services
			static std::recursive_mutex service_mutex_[ServiceIDs::kCount]; //!< Thread safe gets
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void Services::Provide(T* service)
		{
			static_assert(is_service<T>::value, "Provided service type in the service locator is not a service");
			assert(service != nullptr);
			services_[T::SERVICE_ID] = reinterpret_cast<void*>(service);
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline T& Services::Get()
		{
			static_assert(is_service<T>::value, "Service type to retrieve in the service locator is not a service");

			std::lock_guard<std::recursive_mutex> lock(service_mutex_[T::SERVICE_ID]);

			void* ptr = services_[T::SERVICE_ID];
			if (ptr == nullptr)
			{
				static T null_service;
				return null_service;
			}

			return *reinterpret_cast<T*>(ptr);
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void Services::Remove()
		{
			static_assert(std::is_base_of<ServiceBase, T>::value, "The service type to remove a service in the service locator is not a service");
			services_[T::SERVICE_ID] = nullptr;
		}
	}
}