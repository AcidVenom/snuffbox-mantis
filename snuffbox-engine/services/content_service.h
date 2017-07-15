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
		* @author Daniël Konings
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
			* @return (T*) A pointer to the retrieved content, or nullptr if it doesn't exist
			*/
			template <typename T>
			T* Get(const String& path);

			/**
			* @brief Loads and returns a piece of content of type T from a path
			* @param[in] path (const String&) The path to load the content from
			* @return (T*) A pointer to the loaded content, or nullptr if it doesn't exist
			*/
			template <typename T>
			T* Load(const String& path);

			/**
			* @brief Unloads a piece of content of type T from a path
			* @param[in] path (const String&) The path to the already loaded content
			*/
			template <typename T>
			void Unload(const String& path);

		protected:

			/**
			* @see snuffbox::ContentService::Get
			* @remarks param[in] type will be deduced from template argument T
			*/
			virtual ContentBase* GetContent(const String& path, const ContentBase::Types& type);

			/**
			* @see snuffbox::ContentService::Load
			* @remarks param[in] type will be deduced from template argument T
			*/
			virtual ContentBase* LoadContent(const String& path, const ContentBase::Types& type);

			/**
			* @see snuffbox::ContentService::Unload
			* @remarks param[in] type will be deduced from template argument T
			*/
			virtual void UnloadContent(const String& path, const ContentBase::Types& type);
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline T* ContentService::Get(const String& path)
		{
			static_assert(is_content<T>::value, "Attempted to Get content of a non-content type T");
			ContentBase* ptr = GetContent(path, static_cast<ContentBase::Types>(T::CONTENT_ID));
			return static_cast<T*>(ptr);
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline T* ContentService::Load(const String& path)
		{
			static_assert(is_content<T>::value, "Attempted to Load content of a non-content type T");
			ContentBase* ptr = LoadContent(path, static_cast<ContentBase::Types>(T::CONTENT_ID));
			return static_cast<T*>(ptr);
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void ContentService::Unload(const String& path)
		{
			static_assert(is_content<T>::value, "Attempted to Unload content of a non-content type T");
			UnloadContent(path, static_cast<ContentBase::Types>(T::CONTENT_ID));
		}
	}
}