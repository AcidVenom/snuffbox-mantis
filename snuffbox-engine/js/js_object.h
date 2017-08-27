#pragma once

#include <v8.h>

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::JSObject
		* @brief The base class of every class that needs to be exposed to JavaScript
		* @author Daniel Konings
		*/
		class JSObject
		{

		public:

			/**
			* @brief Default constructor
			*/
			JSObject();

			/**
			* @brief Copy constructor, don't copy the persistent handle
			*/
			JSObject(const JSObject& other);

			JSObject operator=(const JSObject& other) { return other; };

			/**
			* @return v8::Persistent<v8::Object>& The persistent handle of this object
			*/
			v8::Persistent<v8::Object>& object();

			/**
			* @brief Default destructor
			*/
			virtual ~JSObject();

		private:

			v8::Persistent<v8::Object> object_; //!< The persistent handle of this object
		};
	}
}
