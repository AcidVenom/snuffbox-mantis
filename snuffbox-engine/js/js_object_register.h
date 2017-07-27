#pragma once

#include "js_wrapper.h"

namespace snuffbox
{
	namespace engine
	{
		class JSStateWrapper;

		/**
		* @struct snuffbox::engine::JSRegister
		* @brief This class will register all Snuffbox classes that need to be exposed to JavaScript
		* @author Daniel Konings
		*/
		struct JSRegister
		{
			
			friend class JSStateWrapper;

		protected:

			/**
			* @brief Registers all singletons
			* @param[in] ns (const v8::Local<v8::Object>&) The namespace to register the object in
			*/
			static void RegisterSingletons(const v8::Local<v8::Object>& ns);

			/**
			* @brief Registers all constructables
			* @param[in] ns (const v8::Local<v8::Object>&) The namespace to register the object in
			*/
			static void RegisterConstructables(const v8::Local<v8::Object>& ns);
			
			/**
			* @brief Registers both singletons and constructables
			*/
			static void Register();
		};

		/**
		* @struct snuffbox::engine::JSObjectRegister<T>
		* @brief Used to register a JavaScript exposed object to the JavaScript state
		* @author Daniel Konings
		*/
		template<typename T>
		struct JSObjectRegister
		{
		public:

			/**
			* @brief Registers the T object as a singleton
			* @param[in] ns (const v8::Local<v8::Object>&) The namespace to register the object in
			*/
			static void RegisterSingleton(const v8::Local<v8::Object>& ns);

			/**
			* @brief Registers the T object as a function template
			* @param[in] ns (const v8::Local<v8::Object>&) The namespace to register the object in
			*/
			static void Register(const v8::Local<v8::Object>& ns);

			/**
			* @brief The 'toString' function available to every existing snuffbox class
			* @param[in] args (const v8::FunctionCallbackInfo<v8::Value>&) The arguments
			*/
			static void ToString(const v8::FunctionCallbackInfo<v8::Value>& args);
		};

		//-----------------------------------------------------------------------------------------------
		template<typename T>
		inline void JSObjectRegister<T>::RegisterSingleton(const v8::Local<v8::Object>& ns)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			v8::HandleScope scope(isolate);

            v8::Local<v8::Object> object = v8::Object::New(isolate);
			T::RegisterJS(object);

            object->Set(wrapper->Context(),
                        JSWrapper::CreateString("toString"),
                        v8::Function::New(wrapper->Context(), JSObjectRegister<T>::ToString).ToLocalChecked());
			
			JSWrapper::SetObjectValue(ns, T::js_name(), object);
		}

		//-----------------------------------------------------------------------------------------------
		template<typename T>
		inline void JSObjectRegister<T>::Register(const v8::Local<v8::Object>& ns)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			v8::HandleScope scope(isolate);

            v8::Local<v8::FunctionTemplate> object = v8::FunctionTemplate::New(isolate);
			T::RegisterJS(object, object->PrototypeTemplate());
			
			object->PrototypeTemplate()->Set(JSWrapper::CreateString("toString"), v8::FunctionTemplate::New(isolate, JSObjectRegister<T>::ToString));
			object->SetCallHandler(JSStateWrapper::New<T>);
			object->SetClassName(JSWrapper::CreateString(T::js_name()));

			JSWrapper::SetObjectValue(ns, T::js_name(), object->GetFunction(wrapper->Context()).ToLocalChecked());
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline void JSObjectRegister<T>::ToString(const v8::FunctionCallbackInfo<v8::Value>& args)
		{
			JSWrapper wrapper(args);

			wrapper.ReturnValue<engine::String>(T::js_name());
		}
	}
}
