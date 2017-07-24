#pragma once

#include "service.h"
#include "services.h"

#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::WindowService : public snuffbox::engine::Service<snuffbox::engine::ServiceIDs::kWindowService>
		*/
		class WindowService : public Service<ServiceIDs::kWindowService>
		{

			friend class Services;

		protected:

			/**
			* @brief Default constructor, creates a null service
			*/
			WindowService();

			/**
			* @brief Delete copy constructor
			*/
			WindowService(const WindowService& other) = delete;

			/**
			* @brief Delete assignment operator
			*/
			WindowService operator=(const WindowService& other) = delete;

		public:

			/**
			* @brief Sets the title of the window
			* @param[in] title (const String&) The new title
			*/
			virtual void SetTitle(const String& title);

			/**
			* @brief Sets the size of the window
			* @param[in] width (unsigned int) The new width
			* @param[in] height (unsigned int) The new height
			*/
			virtual void SetSize(unsigned int width, unsigned int height);

			/**
			* @brief Closes the window
			*/
			virtual void Close();

			/**
			* @return (snuffbox::engine::String) The title of this window
			*/
			virtual String Title() const;

			/**
			* @return The width of the window
			*/
			virtual unsigned int Width() const;

			/**
			* @return The height of the window
			*/
			virtual unsigned int Height() const;

			/**
			* @brief Is the window closed?
			*/
			virtual bool Closed() const;
		};
	}
}
