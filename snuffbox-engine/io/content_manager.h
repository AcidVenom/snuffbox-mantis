#pragma once

#include "../memory/memory.h"
#include "../services/content_service.h"

#include "../js/js_defines.h"

namespace snuffbox
{
	namespace engine
	{
		class CVar;

		/**
		* @class snuffbox::engine::ContentManager : public snuffbox::engine::ContentService
		* @brief The content manager to load, unload and reload all content with
		* @author Daniël Konings
		*/
		class ContentManager : JS_OBJECT_MULTI public ContentService
		{

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

		private:

			typedef Map<String, SharedPtr<ContentBase>> ContentMap;
			ContentMap loaded_content_[ContentBase::Types::kCount]; //!< The currently loaded content per content type

		public:

			JS_NAME_SINGLE(ContentManager);
			JS_FUNCTION_DECL(load);
			JS_FUNCTION_DECL(unload);
		};
	}
}