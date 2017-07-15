#pragma once

#include "content.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::Script
		* @brief The content type that wraps a JavaScript file
		* @author Dani�l Konings
		*/
		class Script : public Content<ContentBase::Types::kScript>
		{
		public:

			/**
			* @see snuffbox::engine::ContentBase::Load
			*/
			bool Load(File* file) override;

			/**
			* @see snuffbox::engine::ContentBase::Reload
			*/
			bool Reload(File* file) override;
		};
	}
}