#pragma once

#include "content.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Script
		* @brief The content type that wraps a JavaScript file
		* @author Daniel Konings
		*/
		class Script : public Content<ContentBase::Types::kScript>
		{
		public:

			/**
			* @see snuffbox::engine::ContentBase::Load
			*/
			bool Load(File* file, ContentManager* cm) override;
		};
	}
}