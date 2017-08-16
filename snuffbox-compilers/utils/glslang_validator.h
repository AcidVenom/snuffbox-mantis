#pragma once

#include <string>
#include <mutex>

#include <glslang/Public/ShaderLang.h>

namespace snuffbox
{
	namespace compilers
	{
		/**
		* @class snuffbox::compilers::GLSLangValidator
		* @brief A utitlity class to help with the compilation of shaders to their respective Spir-V bytecode
		* @remarks This is for compilation only, thus no custom memory allocation is provided as this should not be used within the engine
		* @author Daniel Konings
		*/
		class GLSLangValidator
		{

		public:

			/**
			* @class snuffbox::compilers::Includer : public glslang::TShader::Includer
			* @brief Used to include different shader files across multiple shaders
			* @remarks The parameters and documentation about them can be found in glslang::TShader::Includer
			* @author Daniel Konings
			*/
			class Includer : public glslang::TShader::Includer
			{

			public:
				/**
				* @brief Sets the directory of the includer
				* @param[in] directory (const char*) The directory to search for files
				*/
				Includer(const char* directory);

			protected:
				/**
				* @brief Used to include system directories, this is unused and will always return nullptr
				* @remarks e.g. when the user includes a shader using '#include <path/to/shader>'
				*/
				IncludeResult* includeSystem(const char* header_name, const char* includer_name, size_t depth) override;

				/**
				* @brief Used to include a local directory, this will search from the current root directory of the current shader
				* @remarks e.g. when the user includes a shader using '#include "path/to/shader"'
				*/
				IncludeResult* includeLocal(const char* header_name, const char* includer_name, size_t depth) override;

				/**
				* @brief Releases the used data
				*/
				void releaseInclude(IncludeResult*) override;

			private:

				const char* directory_; //!< The directory to search for files
			};

			/**
			* @brief Used to pass different shader types for compilation
			*/
			enum struct ShaderType : char
			{
				kVertex, //!< A vertex shader
				kPixel, //!< A pixel shader
				kGeometry //!< A geometry shader
			};

			/**
			* @brief Default constructor
			*/
			GLSLangValidator();

			/**
			* @brief Converts raw HLSL shader code into Spir-V
			* @param[in] hlsl (const char*) The HLSL string as binary
			* @param[in] filename (const char*) The file name of the shader being compiled
			* @param[in] type (snuffbox::compilers::GLSLangValidator::ShaderType) The type of the shader
			* @param[out] converted_size (size_t*) The size of the converted Spir-V bytecode
			* @param[out] spirv (unsigned char**) The converted Spir-V bytecode
			* @return (bool) Was the compilation succesful?
			*/
			bool Compile(const char* hlsl, size_t length, const char* filename, ShaderType type, size_t* converted_size, unsigned char** spirv);

			/**
			* @return (const char*) The last error that was encountered, nullptr if no errors have occurred
			*/
			const char* GetError() const;

			/**
			* @brief Frees up the buffer if it was used
			*/
			~GLSLangValidator();

			/**
			* @brief Retrieves the root directory of a file
			* @param[in] full_path (const char*) The full path to the file
			* @return (std::string) The corresponding root directory
			*/
			static std::string GetDirectory(const char* full_path);

			/**
			* @brief Initialises glslang
			*/
			static void Initialise();

			/**
			* @brief Shuts glslang down
			*/
			static void Shutdown();
			
		private:

			static std::mutex GLSLANG_MUTEX_; //!< The mutex for thread safety, I'm not sure how to do actual multithreading using glslang

			std::string error_; //!< The current error message
			unsigned char* buffer_; //!< The buffer to store the Spir-V bytecode in
		};
	}
}