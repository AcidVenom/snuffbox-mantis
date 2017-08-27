#include "glslang_validator.h"

#include <StandAlone/ResourceLimits.h>
#include <SPIRV/GlslangToSpv.h>
#include <glslang/OSDependent/osinclude.h>
#include <fstream>
#include <SPIRV/disassemble.h>

#include <mutex>

namespace snuffbox
{
	namespace compilers
	{
		//-----------------------------------------------------------------------------------------------
		std::mutex GLSLangValidator::GLSLANG_MUTEX_;

		//-----------------------------------------------------------------------------------------------
		GLSLangValidator::Includer::Includer(const char* directory) :
			directory_(directory)
		{

		}

		//-----------------------------------------------------------------------------------------------
		glslang::TShader::Includer::IncludeResult* GLSLangValidator::Includer::includeSystem(const char* header_name, const char* includer_name, size_t depth)
		{
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		glslang::TShader::Includer::IncludeResult* GLSLangValidator::Includer::includeLocal(const char* header_name, const char* includer_name, size_t depth)
		{
			IncludeResult* res = nullptr;

			std::string full_path = std::string(directory_) + "/" + header_name;

			std::ifstream file(full_path.c_str(), std::ios::binary | std::ios::ate);
			if (file) 
			{
				int size = static_cast<int>(file.tellg());
				char* content = new char[size];
				file.seekg(0, file.beg);
				file.read(content, size);
				res = new IncludeResult(header_name, content, size, content);

				file.close();
			}

			return res;
		}

		//-----------------------------------------------------------------------------------------------
		void GLSLangValidator::Includer::releaseInclude(IncludeResult* result)
		{
			if (result != nullptr)
			{
				delete[] static_cast<const char*>(result->userData);
				delete result;
			}
		}

		//-----------------------------------------------------------------------------------------------
		GLSLangValidator::GLSLangValidator() :
			error_(""),
			buffer_(nullptr)
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		bool GLSLangValidator::Compile(const char* hlsl, size_t length, const char* filename, ShaderType type, size_t* converted_size, unsigned char** spirv)
		{
			std::lock_guard<std::mutex> lock(GLSLANG_MUTEX_);

			if (buffer_ != nullptr)
			{
				delete[] buffer_;
				buffer_ = nullptr;
			}

			EShLanguage lang;

			switch (type)
			{
			case ShaderType::kVertex:
				lang = EShLanguage::EShLangVertex;
				break;

			case ShaderType::kPixel:
				lang = EShLanguage::EShLangFragment;
				break;

			case ShaderType::kGeometry:
				lang = EShLanguage::EShLangGeometry;
				break;

			default:
				lang = EShLanguage::EShLangCount;
				break;
			}

			if (lang == EShLanguage::EShLangCount)
			{
				error_ = "Unknown shader type";
				return false;
			}

			const unsigned int spv_version = 0x00001000;
			const int vulkan_version = 100;

			static const char* entry = "main";

			glslang::TProgram program;
			glslang::TShader shader(lang);

			auto Failed = [=](const char* error = nullptr)
			{
				if (error == nullptr && error_.size() == 0)
				{
					error_ = "Unknown error";
				}
				else if (error != nullptr)
				{
					error_ = error;
				}

				return false;
			};

			EShMessages messages =	static_cast<EShMessages>(	EShMessages::EShMsgDefault | 
																EShMessages::EShMsgVulkanRules | 
																EShMessages::EShMsgSpvRules | 
																EShMessages::EShMsgReadHlsl);

			shader.setSourceEntryPoint(entry);
			shader.setEntryPoint(entry);
			shader.setEnvTarget(glslang::EShTargetLanguage::EshTargetSpv, spv_version);
			shader.setEnvClient(glslang::EShClient::EShClientVulkan, vulkan_version);
			shader.setEnvInput(glslang::EShSource::EShSourceHlsl, lang, glslang::EShClient::EShClientVulkan, vulkan_version);

			int l = static_cast<int>(length);
			shader.setStringsWithLengthsAndNames(&hlsl, &l, &filename, 1);
			shader.setShiftSamplerBinding(0);
			shader.setShiftTextureBinding(0);
			shader.setShiftImageBinding(0);
			shader.setShiftUboBinding(0);
			shader.setShiftSsboBinding(0);
			shader.setShiftUavBinding(0);
			shader.setFlattenUniformArrays(false);
			shader.setNoStorageFormat(false);

			std::vector<std::string> rsb;
			shader.setResourceSetBinding(rsb);

			const int desktop_version = 110;

			std::string root = GetDirectory(filename);
			Includer includer(root.c_str());
			bool success = shader.parse(&glslang::DefaultTBuiltInResource, desktop_version, false, messages, includer);

			if (success == false)
			{
				error_ = "Could not compile shader\n\n";
				error_ += shader.getInfoLog();

				return Failed();
			}

			program.addShader(&shader);
			
			if (program.link(messages) == false || program.mapIO() == false)
			{
				error_ = "Could not link shader\n\n";
				error_ += program.getInfoLog();

				return Failed();
			}

			std::vector<unsigned int> spv;
			spv::SpvBuildLogger logger;

			glslang::GlslangToSpv(*program.getIntermediate(lang), spv, &logger);
			
			size_t s = spv.size() * 4;

			if (converted_size != nullptr)
			{
				*converted_size = s;
			}

			buffer_ = new unsigned char[s];

			memcpy(buffer_, &spv[0], s);

			if (spirv != nullptr)
			{
				*spirv = buffer_;
			}

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		const char* GLSLangValidator::GetError() const
		{
			return error_.c_str();
		}

		//-----------------------------------------------------------------------------------------------
		GLSLangValidator::~GLSLangValidator()
		{
			if (buffer_ != nullptr)
			{
				delete[] buffer_;
				buffer_ = nullptr;
			}
		}

		//-----------------------------------------------------------------------------------------------
		std::string GLSLangValidator::GetDirectory(const char* full_path)
		{
			int len = static_cast<int>(strlen(full_path));

			if (len == 0)
			{
				return "";
			}

			int idx = len;
			for (int last = len - 1; last >= 0; --last)
			{
				if (full_path[last] == '/' || full_path[last] == '\\')
				{
					idx = last;
					break;
				}
			}

			if (idx == len)
			{
				return "";
			}

			std::string root(full_path);
			root.erase(root.begin() + idx, root.end());

			return root;
		}

		//-----------------------------------------------------------------------------------------------
		void GLSLangValidator::Initialise()
		{
			glslang::InitializeProcess();
		}

		//-----------------------------------------------------------------------------------------------
		void GLSLangValidator::Shutdown()
		{
			glslang::FinalizeProcess();
		}
	}
}