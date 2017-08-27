#pragma once

#include "service.h"
#include "services.h"

#include "../io/content.h"
#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::ContentService : public snuffbox::engine::Service<snuffbox::engine::ServiceIDs::kContentService>
		* @brief The content service to load, reload and unload content with
		* @author Daniel Konings
		*/
		class ContentService : public Service<ServiceIDs::kContentService>
		{

			friend class Services;

		protected:

			/**
			* @brief Default constructor for the null-service
			*/
			ContentService();

		public:

			/**
			* @brief Delete copy constructor
			*/
			ContentService(const ContentService& other) = delete;

			/**
			* @brief Delete assignment operator
			*/
			ContentService operator=(const ContentService& other) = delete;

			/**
			* @brief Retrieves a piece of content of type T from a path
			* @param[in] path (const String&) The path to retrieve the content from
			* @param[in] quiet (bool) Should this call be quiet and not log anything?
			* @return (snuffbox::engine::ContentPtr<T>) A pointer to the retrieved content, or nullptr if it doesn't exist
			*/
			template <typename T>
			ContentPtr<T> Get(const String& path, bool quiet = false);

			/**
			* @brief Loads and returns a piece of content of type T from a path
			* @param[in] path (const String&) The path to load the content from
			* @param[in] quiet (bool) Should this call be quiet and not log anything?
			* @return (snuffbox::engine::ContentPtr<T>) A pointer to the loaded content, or nullptr if it doesn't exist
			*/
			template <typename T>
			ContentPtr<T> Load(const String& path, bool quiet = false);

			/**
			* @brief Unloads a piece of content of type T from a path
			* @param[in] path (const String&) The path to the already loaded content
			* @param[in] quiet (bool) Should this call be quiet and not log anything?
			*/
			template <typename T>
			void Unload(const String& path, bool quiet = false);

			/**
			* @brief Unload all content
			*/
			virtual void UnloadAll();

		protected:

			/**
			* @see snuffbox::ContentService::Get
			* @remarks param[in] type will be deduced from template argument T
			*/
			virtual ContentPtr<ContentBase> GetContent(const String& path, ContentBase::Types type, bool quiet);

			/**
			* @see snuffbox::ContentService::Load
			* @remarks param[in] type will be deduced from template argument T
			*/
			virtual ContentPtr<ContentBase> LoadContent(const String& path, ContentBase::Types type, bool quiet);

			/**
			* @see snuffbox::ContentService::Unload
			* @remarks param[in] type will be deduced from template argument T
			*/
			virtual void UnloadContent(const String& path, ContentBase::Types type, bool quiet);
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline ContentPtr<T> ContentService::Get(const String& path, bool quiet)
		{
			static_assert(is_content<T>::value, "Attempted to Get content of a non-content type T");
			ContentPtr<ContentBase> ptr = GetContent(path, static_cast<ContentBase::Types>(T::CONTENT_ID), quiet);
			return ContentPtr<T>(ptr);
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline ContentPtr<T> ContentService::Load(const String& path, bool quiet)
		{
			static_assert(is_content<T>::value, "Attempted to Load content of a non-content type T");
			ContentPtr<ContentBase> ptr = LoadContent(path, static_cast<ContentBase::Types>(T::CONTENT_ID), quiet);
			return ContentPtr<T>(ptr);
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void ContentService::Unload(const String& path, bool quiet)
		{
			static_assert(is_content<T>::value, "Attempted to Unload content of a non-content type T");
			UnloadContent(path, static_cast<ContentBase::Types>(T::CONTENT_ID), quiet);
		}
	}
}