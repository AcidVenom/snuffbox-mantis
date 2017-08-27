#pragma once

#include "compiler.h"

namespace snuffbox
{
	namespace compilers
	{
		/**
		* @class snuffbox::compilers::ScriptCompiler : public snuffbox::compilers::Compiler
		* @brief Compiles or decompiles JavaScript files with RC4 encryption
		* @author Daniël Konings
		*/
		class ScriptCompiler : public Compiler
		{

		public:

			/**
			* @see snuffbox::compilers::Compiler::Compiler
			*/
			ScriptCompiler(Allocation allocator = nullptr, Deallocation deallocator = nullptr);


		protected:

			/**
			* @see snuffbox::compilers::Compiler::Compilation
			*/
			bool Compilation(const unsigned char* input, size_t size, size_t* out_size, const unsigned char* userdata) override;

			/**
			* @see snuffbox::compilers::Compiler::Decompilation
			*/
			bool Decompilation(const unsigned char* input, size_t* out_size, const unsigned char* userdata) override;
		};
	}
}