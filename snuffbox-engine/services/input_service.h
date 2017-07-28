#pragma once

#include "service.h"
#include "services.h"

#include "../input/keycodes.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::InputService : public snuffbox::engine::Service<snuffbox::engine::ServiceIDs::kInputService>
		* @brief The input service to retrieve input data from the keyboard, mouse and controller with
		* @author Daniel Konings
		*/
		class InputService : public Service<ServiceIDs::kInputService>
		{
			friend class Services;

		protected:

			/**
			* @brief Default constructor, constructs a null-service
			*/
			InputService();

		public:

			/**
			* @brief Delete copy constructor
			*/
			InputService(const InputService& other) = delete;

			/**
			* @brief Delete assignment operator
			*/
			InputService operator=(const InputService& other) = delete;

			
		};
	}
}