#pragma once

#include "../memory/memory.h"
#include "../services/content_service.h"

#include "../js/js_defines.h"
#include "file_watch.h"

namespace snuffbox
{
	namespace graphics
	{
		class Renderer;
	}

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
			* @param[in] renderer (snuffbox::graphics::Renderer*) The current renderer
			* @param[in] app (snuffbox::engine::SnuffboxApp*) The current application
			*/
			void Initialise(CVar* cvar, graphics::Renderer* renderer, SnuffboxApp* app);

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
			ContentPtr<ContentBase> GetContent(const String& path, ContentBase::Types type, bool quiet) override;

			/**
			* @see snuffbox::engine::ContentService::LoadContent
			*/
			ContentPtr<ContentBase> LoadContent(const String& path, ContentBase::Types type, bool quiet) override;

			/**
			* @see snuffbox::engine::ContentService::UnloadContent
			*/
			void UnloadContent(const String& path, ContentBase::Types type, bool quiet) override;

			/**
			* @see snuffbox::engine::ContentService::UnloadAll
			*/
			void UnloadAll() override;

			/**
			* @brief Concatenates a full path string from a relative path
			* @param[in] path (const snuffbox::engine::String&) The path to concatenate
			* @return (snuffbox::engine::String) The full path including source directory
			*/
			String FullPath(const String& path) const;

		public:

			/**
			* @return (snuffbox::graphics::Renderer*) The current renderer
			*/
			graphics::Renderer* renderer() const;

		private:

			typedef Map<String, ContentPtr<ContentBase>> ContentMap;
			ContentMap loaded_content_[ContentBase::Types::kCount]; //!< The currently loaded content per content type

			String src_directory_; //!< The working directory
			FileWatch watch_; //!< The file watch

			graphics::Renderer* renderer_; //!< The current renderer
			SnuffboxApp* application_; //!< The current application

		public:

			JS_NAME_SINGLE(ContentManager);
			JS_FUNCTION_DECL(load);
			JS_FUNCTION_DECL(get);
			JS_FUNCTION_DECL(unload);
			JS_FUNCTION_DECL(unloadAll);
		};
	}
}