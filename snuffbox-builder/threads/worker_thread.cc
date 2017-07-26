#include "worker_thread.h"
#include "build_thread.h"

#include <fstream>
#include <assert.h>

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		WorkerThread::WorkerThread(BuildThread* build_thread) :
			build_thread_(build_thread),
			has_error_(false),
			error_(""),
			id_(-1),
			finished_(true)
		{
			assert(build_thread_ != nullptr);

			compilers_[static_cast<int>(FileType::kScript)] = new compilers::ScriptCompiler();
		}

		//-----------------------------------------------------------------------------------------------
		void WorkerThread::Reset()
		{
			finished_ = false;
			has_error_ = false;
			error_ = "";
		}

		//-----------------------------------------------------------------------------------------------
		void WorkerThread::SetError(const std::string& error, const std::string& compiling)
		{
			finished_ = true;
			has_error_ = true;
			error_ = error;

			build_thread_->OnCompiled(this, compiling);
		}

		//-----------------------------------------------------------------------------------------------
		void WorkerThread::Run(int id, const BuildCommand& cmd)
		{
			assert(build_thread_ != nullptr);

			Join();

			id_ = id;
			command_ = cmd;
			
			Reset();

			thread_ = std::thread([=]()
			{
				size_t file_size;
				size_t out_size;
				unsigned char* input = OpenFile(command_.src_path, &file_size);

				if (input == nullptr)
				{
					SetError("Could not open file", command_.src_path);
					return;
				}

				const unsigned char* output = nullptr;

				bool compiled = false;

				compiled = compilers_[static_cast<int>(command_.file_type)]->Compile(input, file_size, &out_size, &output);
				
				free(input);

				if (compiled == false)
				{
					SetError("Could not compile", command_.src_path);
					return;
				}

				std::ofstream fout(command_.build_path, std::ios::binary);

				if (fout.is_open() == false)
				{
					SetError("Could not save", command_.src_path);
					return;
				}

				fout.write(reinterpret_cast<const char*>(output), out_size);
				fout.close();

				build_thread_->OnCompiled(this, command_.src_path);
				finished_ = true;
			});
		}

		//-----------------------------------------------------------------------------------------------
		void WorkerThread::Join()
		{
			if (thread_.joinable() == true)
			{
				thread_.join();
			}
		}

		//-----------------------------------------------------------------------------------------------
		unsigned char* WorkerThread::OpenFile(const std::string& path, size_t* file_size)
		{
			std::ifstream fin(path.c_str(), std::ios::binary);

			if (fin.is_open() == false)
			{
				return nullptr;
			}

			std::streampos size = fin.tellg();
			fin.seekg(0, std::ios::end);
			size = fin.tellg() - size;

			fin.seekg(0, std::ios::beg);

			char* buffer = reinterpret_cast<char*>(malloc(size));
			fin.read(buffer, size);

			fin.close();

			if (file_size != nullptr)
			{
				*file_size = size;
			}

			return reinterpret_cast<unsigned char*>(buffer);
		}

		//-----------------------------------------------------------------------------------------------
		const std::string& WorkerThread::GetError(bool* has_error) const
		{
			if (has_error != nullptr)
			{
				*has_error = has_error_;
			}

			return error_;
		}

		//-----------------------------------------------------------------------------------------------
		const int& WorkerThread::id() const
		{
			return id_;
		}

		//-----------------------------------------------------------------------------------------------
		const bool& WorkerThread::finished() const
		{
			return finished_;
		}

		//-----------------------------------------------------------------------------------------------
		WorkerThread::~WorkerThread()
		{
			for (int i = 0; i < static_cast<int>(FileType::kSkip); ++i)
			{
				delete compilers_[i];
				compilers_[i] = nullptr;
			}
		}
	}
}