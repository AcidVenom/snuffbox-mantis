#pragma once

#include <v8.h>

#include "../core/eastl.h"

#include "js_defines.h"
#include "js_allocator.h"

namespace snuffbox
{
	namespace engine
	{
		class SnuffboxApp;
		class LogClient;
		class JSWrapper;
		struct JSFunctionRegister;

		/**
		* @class snuffbox::engine::JSStateWrapper
		* @brief The state wrapper for the JavaScript state
		* @author Daniël Konings
		*/
		class JSStateWrapper
		{

			friend class SnuffboxApp;
			friend class Allocator;
			friend class LogClient;
			friend class JSWrapper;
			friend struct JSFunctionRegister;

		protected:

			/**
			* @brief Default constructor
			* @param[in] allocator (snuffbox::engine::Allocator&) The allocator to use for the JavaScript state
			*/
			JSStateWrapper(Allocator& allocator);

			/**
			* @brief Initialises V8 and the JavaScript context
			*/
			void Initialise();

			/**
			* @brief Registers all common JavaScript functions (e.g. require, assert, etc.)
			*/
			void RegisterCommon();

			/**
			* @brief Creates the global scope and returns it
			* @return (v8::Handle<v8::ObjectTemplate>) The global scope
			*/
			v8::Handle<v8::ObjectTemplate> CreateGlobal() const;

			/**
			* @brief Creates the context and returns it
			* @return (v8::Handle<v8::Context>) The context
			*/
			v8::Handle<v8::Context> CreateContext(const v8::Handle<v8::ObjectTemplate>& global) const;

			/**
			* @brief Retrieves an error from a v8::TryCatch, if there is any
			* @param[in] try_catch (v8::TryCatch) The try catch object to retrieve the error from
			* @param[out] out (snuffbox::engine::String*) The output, if there is an error
			* @return (bool) Was there an error?
			*/
			bool GetException(v8::TryCatch* try_catch, engine::String* out) const;

			/**
			* @brief Retrieves a stack track by a maximum number of calls
			* @param[in] max (const unsigned int&) The maximum number of calls to retrieve from the current stack
			* @return (engine::String) The stack trace as a string
			*/
			engine::String StackTrace(const unsigned int& max = 10) const;

			/**
			* @brief Collects all garbage
			*/
			void CollectGarbage();

			/**
			* @brief Disposes V8
			*/
			void Dispose();

			/**
			* @brief Shuts down and disposes V8
			* @remarks This also garbage collects any remaining memory
			*/
			void Shutdown();

			/**
			* @return (v8::Local::v8::Object>) The global scope of this JavaScript state
			*/
			v8::Local<v8::Object> Global() const;

			/**
			* @return (v8::Isolate*) The isolated memory heap for this JavaScript state
			*/
			v8::Isolate* isolate() const;

		public:

			/**
			* @brief Runs a specified piece of code from a virtual file
			* @param[in] src (const snuffbox::engine::String&) The JavaScript code to execute
			* @param[in] file_name (const snuffbox::engine::String&) The file name in which this piece of code runs
			* @param[in] echo (const bool&) Should the result be echo'd to the console?
			* @return (bool) Was the execution succesful?
			*/
			bool Run(const engine::String& src, const engine::String& file_name, const bool& echo) const;

			/**
			* @return (snuffbox::engine::JSStateWrapper*) The current instance
			*/
			static JSStateWrapper* Instance();

		private:

			JS_FUNCTION_DECL(require);
			JS_FUNCTION_DECL(assert);

			JSAllocator allocator_; //!< The allocator of this JavaScript state

			v8::Isolate* isolate_; //!< The isolated memory heap for this JavaScript state
			v8::Persistent<v8::Context> context_; //!< The context we will use for this JavaScript state
			v8::Persistent<v8::ObjectTemplate> global_; //!< The global scope for use with the JavaScript state
			v8::Platform* platform_; //!< The V8 platform

			static JSStateWrapper* instance_; //!< The current instance
		};
	}
}