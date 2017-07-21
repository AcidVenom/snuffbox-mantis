#pragma once

#include "../memory/memory.h"
#include "../services/content_service.h"

#include "../js/js_defines.h"
#include "file_watch.h"

namespace snuffbox
{
	namespace engine
	{
		class CVar;
		class SnuffboxApp;
		class FileWatch;

		/**
		* @class snuffbox::engine::ContentManager : [JSObject] public snuffbox::engine::ContentService
		* @brief The content manager to load, unload and reload all content with
		* @author Daniel Konings
		*/
		class ContentManager : JS_OBJECT_MULTI public ContentService
		{

			friend class SnuffboxApp;
			friend class Allocator;
			friend class FileWatch;

		protected:

			/**
			* @brief Default constructor
			*/
			ContentManager();

			/**
			* @brief Initialises the content manager
			* @param[in] cvar (snuffbox::engine::CVar*) The CVar system
			*/
			void Initialise(CVar* cvar);

			/**
			* @brief Reloads a loaded file in the content manager
			* @param[in] path (const snuffbox::engine::String&) The path to the file to reload as stored in the content map
			*/
			void Reload(const String& path);

			/**
			* @brief Updates the file watch
			*/
			void Update();

			/**
			* @see snuffbox::engine::ContentService::GetContent
			*/
			ContentBase* GetContent(const String& path, const ContentBase::Types& type) override;

			/**
			* @see snuffbox::engine::ContentService::LoadContent
			*/
			ContentBase* LoadContent(const String& path, const ContentBase::Types& type) override;

			/**
			* @see snuffbox::engine::ContentService::UnloadContent
			*/
			void UnloadContent(const String& path, const ContentBase::Types& type) override;

			/**
			* @brief Concatenates a full path string from a relative path
			* @param[in] path (const snuffbox::engine::String&) The path to concatenate
			* @return (snuffbox::engine::String) The full path including source directory
			*/
			String FullPath(const String& path) const;

		private:

			typedef Map<String, SharedPtr<ContentBase>> ContentMap;
			ContentMap loaded_content_[ContentBase::Types::kCount]; //!< The currently loaded content per content type

			String src_directory_; //!< The working directory
			FileWatch watch_; //!< The file watch

		public:

			JS_NAME_SINGLE(ContentManager);
			JS_FUNCTION_DECL(load);
			JS_FUNCTION_DECL(unload);
		};
	}
}