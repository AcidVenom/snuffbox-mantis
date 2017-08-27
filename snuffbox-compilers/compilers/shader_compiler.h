#pragma once

#include "compiler.h"

namespace snuffbox
{
	namespace compilers
	{
		/**
		* @class snuffbox::compilers::ShaderCopmiler : public snuffbox::compilers::Compiler
		* @brief Used to compile HLSL shader code into both GLSL/Spir-V
		* @author Daniel Konings
		*/
		class ShaderCompiler : public Compiler
		{

		public:

			/**
			* @struct snuffbox::compilers::ShaderCompiler::Header
			* @brief A header to contain shader information in
			* @author Daniel Konings
			*/
			struct Header
			{
				size_t size; //!< The size of the shader byte code
				char type; //!< The type of the shader
			};

			/**
			* @see snuffbox::compilers::Compiler::Compiler
			*/
			ShaderCompiler(Allocation allocator = nullptr, Deallocation deallocator = nullptr);

		protected:

			/**
			* @see snuffbox::compilers::Compiler::Compilation
			* @remarks 'userdata' should be the path to the shader
			*/
			bool Compilation(const unsigned char* input, size_t size, size_t* out_size, const unsigned char* userdata) override;

			/**
			* @see snuffbox::compilers::Compiler::Decompilation
			*/
			bool Decompilation(const unsigned char* input, size_t* out_size, const unsigned char* userdata) override;

			/**
			* @brief Retrieves the shader type from an extension
			* @param[in] path (const char*) The path to read the extension from
			* @return (char) The GLSLangValidator shader type
			*/
			static char GetShaderTypeFromExtension(const char* path);
		};
	}
}