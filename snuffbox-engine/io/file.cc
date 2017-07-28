#include "file.h"

#include "../services/log_service.h"
#include "../services/cvar_service.h"

#include "../memory/memory.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		File::File() :
			file_(nullptr),
			path_(""),
			relative_(false),
			buffer_(nullptr)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void File::Read(bool null_terminated)
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

            fseek(file_, 0, SEEK_END);
			size_t size = ftell(file_);

			if (size == 0)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kWarning, "File '{0}' was empty", path_);
				return;
			}

			size_t new_size = null_terminated == true ? size + 1 : size;
			buffer_ = reinterpret_cast<unsigned char*>(Memory::default_allocator().Malloc(new_size));

            fseek(file_, 0, SEEK_SET);
			fread(buffer_, size, file_);

			if (null_terminated == true)
			{
				memset(buffer_ + size, '\0', sizeof(char));
			}
		}

		//-----------------------------------------------------------------------------------------------
		File* File::Open(const engine::String& path, unsigned int flags, bool relative, File* opened)
		{
			File* file = opened == nullptr ? Memory::default_allocator().Construct<File>() : opened;

			engine::String mode = "";

			mode += (flags & AccessFlags::kRead) == AccessFlags::kRead ? "r" : "";
			mode += (flags & AccessFlags::kWrite) == AccessFlags::kWrite ? "w" : "";
			mode += (flags & AccessFlags::kBinary) == AccessFlags::kBinary ? "b" : "";

			CVarString* src = Services::Get<CVarService>().Get<CVarString>("src_directory");
			engine::String full_path = relative == false ? path : (src == nullptr || src->value() == "" ? path : src->value() + '/' + path);

			fopen(file->file_, path.c_str(), mode.c_str());

			if (file->file_ == nullptr)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kError, "Could not open file '{0}' or the access flags don't allow it to be opened", path);
			}

			file->path_ = path;
			file->relative_ = relative;

			return file;
		}

		//-----------------------------------------------------------------------------------------------
		void File::Close(File* file)
		{
			Memory::default_allocator().Destruct<File>(file);
		}

		//-----------------------------------------------------------------------------------------------
		const unsigned char* File::Binary()
		{
			Read(false);
			return buffer_;
		}

		//-----------------------------------------------------------------------------------------------
		const char* File::String()
		{
			Read(true);
			return reinterpret_cast<const char*>(buffer_);
		}

		//-----------------------------------------------------------------------------------------------
		bool File::Write(const unsigned char* data, size_t size)
		{
			if (file_ == nullptr)
			{
				return false;
			}

			fclose(file_);
			remove(path_.c_str());

			Open(path_.c_str(), File::AccessFlags::kWrite | File::AccessFlags::kBinary, relative_, this);

			if (file_ == nullptr)
			{
				return false;
			}

			fwrite(data, sizeof(unsigned char), size, file_);

			return true;
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

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_TMPL(File, JS_BODY(
		{
			JSFunctionRegister funcs[] = 
			{
				JS_FUNCTION_REG(open),
				JS_FUNCTION_REG(read),
				JS_FUNCTION_REG(write),
				JS_FUNCTION_REG(close),
				JS_FUNCTION_REG_END
			};

			JSWrapper::SetFunctionTemplateValue(func, "Read", static_cast<unsigned int>(File::AccessFlags::kRead));
			JSWrapper::SetFunctionTemplateValue(func, "Write", static_cast<unsigned int>(File::AccessFlags::kWrite));
			JSWrapper::SetFunctionTemplateValue(func, "ReadWrite", static_cast<unsigned int>(File::AccessFlags::kRead | File::AccessFlags::kWrite));

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_CONSTRUCTOR(File, JS_BODY(
		{
			JSWrapper wrapper(args);

			file_ = nullptr;
			path_ = "";
			buffer_ = nullptr;
			relative_ = false;

		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(File, open, JS_BODY(
		{
			JS_SETUP(File);

			if (wrapper.Check("SN") == true)
			{
				engine::String path = wrapper.GetValue<engine::String>(0, "");
				unsigned int flags = wrapper.GetValue<unsigned int>(1, static_cast<unsigned int>(File::AccessFlags::kRead));

				File::Open(path, flags, wrapper.GetValue<bool>(2, false), self);
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(File, read, JS_BODY(
		{
			JS_SETUP(File);

			const char* buffer = self->String();
			engine::String contents = buffer;

			if (buffer == nullptr)
			{
				wrapper.ReturnValue<bool>(false);
				return;
			}

			wrapper.ReturnValue<engine::String>(contents);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(File, write, JS_BODY(
		{
			JS_SETUP(File);

			if (wrapper.Check("S") == true)
			{
				engine::String to_write = wrapper.GetValue<engine::String>(0, "");
				self->Write(reinterpret_cast<const unsigned char*>(to_write.c_str()), to_write.size());
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(File, close, JS_BODY(
		{
			JS_SETUP(File);

			self->~File();
		}));
	}
}
