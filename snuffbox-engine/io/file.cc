#include "file.h"

#include "../services/log_service.h"
#include "../memory/memory.h"

#ifdef SNUFF_WIN32
#define fopen(out, path, flags) fopen_s(&out, path, flags);
#define fread(out, size, file) fread_s(out, size, size, 1, file)
#else
#define fopen(out, path, flags) out = fopen(path, flags);
#define fread(out, size, file) fread(&out, size, 1, file);
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		File::File() :
			file_(nullptr),
			path_(""),
			buffer_(nullptr)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void File::Read()
		{
			if (file_ == nullptr)
			{
				return;
			}

			if (buffer_ != nullptr)
			{
				Memory::default_allocator().Free(buffer_);
				buffer_ = nullptr;
			}

			fseek(file_, 0L, SEEK_END);
			size_t size = ftell(file_);

			if (size == 0)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kWarning, "File '{0}' was empty", path_.c_str());
				return;
			}

			buffer_ = reinterpret_cast<unsigned char*>(Memory::default_allocator().Malloc(size + 1));

			fseek(file_, 0L, SEEK_SET);
			fread(buffer_, size, file_);

			memset(buffer_ + size, '\0', sizeof(char));
		}

		//-----------------------------------------------------------------------------------------------
		File* File::Open(const engine::String& path, const unsigned int& flags)
		{
			File* file = Memory::default_allocator().Construct<File>();
			engine::String mode = "";

			mode += (flags & AccessFlags::kRead) == AccessFlags::kRead ? "r" : "";
			mode += (flags & AccessFlags::kWrite) == AccessFlags::kWrite ? "w" : "";
			mode += (flags & AccessFlags::kBinary) == AccessFlags::kBinary ? "b" : "";

			fopen(file->file_, path.c_str(), mode.c_str());

			if (file->file_ == nullptr)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kError, "Could not open file '{0}' or the access flags don't allow it to be opened", path.c_str());
			}

			file->path_ = path;

			return file;
		}

		//-----------------------------------------------------------------------------------------------
		void File::Close(File* file)
		{
			assert(file != nullptr);
			Memory::default_allocator().Destruct<File>(file);
		}

		//-----------------------------------------------------------------------------------------------
		const unsigned char* File::Binary()
		{
			Read();
			return buffer_;
		}

		//-----------------------------------------------------------------------------------------------
		const char* File::String()
		{
			Read();
			return reinterpret_cast<const char*>(buffer_);
		}

		//-----------------------------------------------------------------------------------------------
		const String& File::path() const
		{
			return path_;
		}

		//-----------------------------------------------------------------------------------------------
		File::~File()
		{
			if (file_ != nullptr)
			{
				fclose(file_);
			}

			if (buffer_ != nullptr)
			{
				Memory::default_allocator().Free(buffer_);
			}
		}
	}
}