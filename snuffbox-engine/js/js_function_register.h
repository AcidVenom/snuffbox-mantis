#pragma once

#include <v8.h>

namespace snuffbox
{
	namespace engine
	{
		/**
		* @struct snuffbox::engine::JSFunctionRegister
		* @brief A description to expose different C++ functions to JavaScript
		* @author Daniël Konings
		*/
		struct JSFunctionRegister
		{
			const char* name; //!< The name of the function in JavaScript
			v8::FunctionCallback function; //!< The C++ function to register

			/**
			* @brief Registers the provided functions to an object template
			* @param[in] funcs (snuffbox::engine::JSFunctionRegister*) The functions to register
            * @param[in] obj (const v8::Local<v8::ObjectTemplate>&) The object template to register the functions to
			* @remarks The functions should contain a null-terminated function register for counting
			*/
            static void Register(JSFunctionRegister* funcs, const v8::Local<v8::ObjectTemplate>& obj);

			/**
			* @brief Registers the provided functions to an object
			* @param[in] funcs (snuffbox::engine::JSFunctionRegister*) The functions to register
            * @param[in] obj (const v8::Local<v8::Object>&) The object to register the functions to
			* @remarks The functions should contain a null-terminated function register for counting
			*/
            static void Register(JSFunctionRegister* funcs, const v8::Local<v8::Object>& obj);

			/**
			* @brief Registers the provided functions in the global scope
			* @param[in] funcs (snuffbox::engine::JSFunctionRegister*) The functions to register
			* @remarks The functions should contain a null-terminated function register for counting
			*/
			static void Register(JSFunctionRegister* funcs);
		};
	}
}
