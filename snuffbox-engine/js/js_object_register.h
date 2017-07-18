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
		* @author Daniël Konings
		*/
		struct JSRegister
		{
			
			friend class JSStateWrapper;

		protected:
			
			/**
			* @brief Registers all singletons
			*/
			static void RegisterSingletons();

			/**
			* @brief Registers all constructables
			*/
			static void RegisterConstructables();
			
			/**
			* @brief Registers both singletons and constructables
			*/
			static void Register();
		};

		/**
		* @struct snuffbox::engine::JSObjectRegister<T>
		* @brief Used to register a JavaScript exposed object to the JavaScript state
		* @author Daniël Konings
		*/
		template<typename T>
		struct JSObjectRegister
		{
		public:

			/**
			* @brief Registers the T object as a singleton
			*/
			static void RegisterSingleton();

			/**
			* @brief Registers the T object as a function template
			*/
			static void Register();

			/**
			* @brief The 'toString' function available to every existing snuffbox class
			* @param[in] args (const v8::FunctionCallbackInfo<v8::Value>&) The arguments
			*/
			static void ToString(const v8::FunctionCallbackInfo<v8::Value>& args);
		};

		//-----------------------------------------------------------------------------------------------
		template<typename T>
		inline void JSObjectRegister<T>::RegisterSingleton()
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			v8::HandleScope scope(isolate);

            v8::Local<v8::Object> object = v8::Object::New(isolate);
			T::RegisterJS(object);

            object->Set(wrapper->Context(),
                        v8::String::NewFromUtf8(isolate, "toString", v8::NewStringType::kNormal).ToLocalChecked(),
                        v8::Function::New(wrapper->Context(), JSObjectRegister<T>::ToString).ToLocalChecked());
			
			wrapper->RegisterGlobal(T::js_name(), object);
		}

		//-----------------------------------------------------------------------------------------------
		template<typename T>
		inline void JSObjectRegister<T>::Register()
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			v8::HandleScope scope(isolate);

            v8::Local<v8::FunctionTemplate> object = v8::FunctionTemplate::New(isolate);
			T::RegisterJS(object->PrototypeTemplate());
			
			object->PrototypeTemplate()->Set(v8::String::NewFromUtf8(isolate, "toString"), v8::FunctionTemplate::New(isolate, JSObjectRegister<T>::ToString));
			object->SetCallHandler(JSStateWrapper::New<T>);
			object->SetClassName(v8::String::NewFromUtf8(isolate, T::js_name()));

			wrapper->RegisterGlobal(T::js_name(), object->GetFunction());
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
