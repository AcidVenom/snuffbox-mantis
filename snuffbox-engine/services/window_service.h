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

		public:

			/**
			* @brief Sets the title of the window
			* @param[in] title (const String&) The new title
			*/
			virtual void SetTitle(const String& title);

			/**
			* @brief Sets the size of the window
			* @param[in] width (const unsigned int&) The new width
			* @param[in] height (const unsigned int&) The new height
			*/
			virtual void SetSize(const unsigned int& width, const unsigned int& height);

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
		};
	}
}
