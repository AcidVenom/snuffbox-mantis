#pragma once

#include "content.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Shader
		* @brief The content type that wraps a shader file
		* @author Daniel Konings
		*/
		class Shader : public Content<ContentBase::Types::kShader>
		{

		public:

			/**
			* @brief Default constructor
			*/
			Shader();

			/**
			* @see snuffbox::engine::ContentBase::Load
			*/
			bool Load(File* file, ContentManager* cm) override;

			/**
			* @see snuffbox::engine::ContentBase::Reload
			*/
			bool Reload(File* file, ContentManager* cm) override;

			/**
			* @see snuffbox::engine::ContentBase::Unload
			*/
			void Unload(ContentManager* cm) override;

		protected:

			/**
			* @brief Releases the underlying shader blob if it exists
			* @param[in] cm (snuffbox::engine::ContentManager*) The current content manager
			*/
			void Release(ContentManager* cm);

		private:

			void* blob_; //!< The platform specific shader blob
		};
	}
}