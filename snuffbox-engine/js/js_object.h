#pragma once

#include <v8.h>

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::JSObject
		* @brief The base class of every class that needs to be exposed to JavaScript
		* @author Daniël Konings
		*/
		class JSObject
		{

		public:

			/**
			* @return v8::Persistent<v8::Object>& The persistent handle of this object
			*/
			v8::Persistent<v8::Object>& object();

			/**
			* @brief Default destructor
			*/
			virtual ~JSObject();

		private:

			v8::Persistent<v8::Object> object_; //!< THe persistent handle of this object
		};
	}
}
