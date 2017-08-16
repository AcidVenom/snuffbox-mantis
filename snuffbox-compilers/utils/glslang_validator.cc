#include "glslang_validator.h"

#include <StandAlone/ResourceLimits.h>
#include <SPIRV/GlslangToSpv.h>
#include <glslang/OSDependent/osinclude.h>
#include <fstream>

namespace snuffbox
{
	namespace compilers
	{
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
			glslang::InitializeProcess();
		}

		//-----------------------------------------------------------------------------------------------
		bool GLSLangValidator::Compile(const char* hlsl, size_t length, const char* filename, ShaderType type, size_t* converted_size, unsigned char** spirv)
		{
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

			glslang::SpvVersion version;
			version.spv = 0x00001000;
			version.vulkan = 100;

			static const char* entry = "main";

			glslang::TProgram* program = new glslang::TProgram();
			glslang::TShader* shader = new glslang::TShader(lang);

			shader->setSourceEntryPoint(entry);
			shader->setEntryPoint(entry);
			shader->setEnvTarget(glslang::EShTargetLanguage::EshTargetSpv, version.spv);
			shader->setEnvClient(glslang::EShClient::EShClientVulkan, version.vulkan);
			shader->setEnvInput(glslang::EShSource::EShSourceHlsl, lang, glslang::EShClient::EShClientVulkan, version.vulkan);
			shader->setHlslIoMapping(true);

			int l = static_cast<int>(length);
			shader->setStringsWithLengthsAndNames(&hlsl, &l, &filename, 1);
			shader->setShiftSamplerBinding(0);
			shader->setShiftTextureBinding(0);
			shader->setShiftImageBinding(0);
			shader->setShiftUboBinding(0);
			shader->setShiftSsboBinding(0);
			shader->setShiftUavBinding(0);
			shader->setFlattenUniformArrays(false);
			shader->setNoStorageFormat(false);

			std::string root = GetDirectory(filename);
			Includer includer(root.c_str());
			bool success = shader->parse(&glslang::DefaultTBuiltInResource, version.vulkan, true, EShMessages::EShMsgDefault, includer);

			if (success == false)
			{
				error_ = "Could not compile shader\n\n";
				error_ += shader->getInfoLog();

				delete program;
				delete shader;
				return false;
			}

			program->addShader(shader);
			
			if (program->link(EShMessages::EShMsgDefault) == false || program->mapIO() == false)
			{
				error_ = "Could not link shader\n\n";
				error_ += program->getInfoLog();

				delete program;
				delete shader;
				return false;
			}

			std::vector<unsigned int> spv;
			spv::SpvBuildLogger logger;

			glslang::GlslangToSpv(*program->getIntermediate(lang), spv, &logger);
			
			size_t s = spv.size() * 4;

			if (converted_size != nullptr)
			{
				*converted_size = s;
			}

			buffer_ = new unsigned char[s];

			unsigned int word;
			unsigned char offset = 0;

			for (int i = 0; i < static_cast<int>(spv.size()); ++i)
			{
				word = spv[i];
				memcpy(buffer_ + offset, reinterpret_cast<const char*>(&word), 4);
				offset += 4;
			}

			if (spirv != nullptr)
			{
				*spirv = buffer_;
			}

			delete program;
			delete shader;

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		const char* GLSLangValidator::GetError() const
		{
			if (error_.size() == 0)
			{
				return nullptr;
			}

			return error_.c_str();
		}

		//-----------------------------------------------------------------------------------------------
		GLSLangValidator::~GLSLangValidator()
		{
			if (buffer_ != nullptr)
			{
				free(buffer_);
				buffer_ = nullptr;
			}

			glslang::FinalizeProcess();
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
	}
}