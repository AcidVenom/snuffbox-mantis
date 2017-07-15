#pragma once

#include "content.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Script
		* @brief The content type that wraps a JavaScript file
		* @author Daniël Konings
		*/
		class Script : public Content<ContentBase::Types::kScript>
		{
		public:

			bool Load(File* file) override;
			bool Reload(File* file) override;
		};
	}
}