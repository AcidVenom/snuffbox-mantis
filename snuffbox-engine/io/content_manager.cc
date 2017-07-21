#include "content_manager.h"
#include "file.h"

#include "../services/log_service.h"
#include "../logging/cvar.h"

#include "script.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		ContentManager::ContentManager() :
			watch_(FileWatch(this))
		{

		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::Initialise(CVar* cvar)
		{
			LogService& log = Services::Get<LogService>();
			CVarString* src = cvar->Get<CVarString>("src_directory");

			if (src != nullptr)
			{
				src_directory_ = src->value();

				log.Log(console::LogSeverity::kInfo, "Set the source directory of the project to '{0}'", src_directory_);

				return;
			}
			else
			{
				src_directory_ = "";
				cvar->Set<CVarString>("src_directory", "");

				log.Log(console::LogSeverity::kInfo, "Set the source directory of the project to the target root directory");
			}
		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::Reload(const String& path)
		{
			for (int i = 0; i < ContentBase::Types::kCount; ++i)
			{
				ContentMap& map = loaded_content_[i];
				
				ContentMap::iterator it = map.find(path);
				if (it != map.end())
				{
					File* f = File::Open(path, File::AccessFlags::kRead | File::AccessFlags::kBinary);
					it->second->Reload(f);
					File::Close(f);

					break;
				}
			}

			Services::Get<LogService>().Log(console::LogSeverity::kInfo, "Reloaded file: '{0}'", path);
		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::Update()
		{
			CVarBoolean* reload = Services::Get<CVarService>().Get<CVarBoolean>("reload");

			if (reload != nullptr && reload->value() == true)
			{
				watch_.Update();
			}
		}

		//-----------------------------------------------------------------------------------------------
		ContentBase* ContentManager::GetContent(const String& path, const ContentBase::Types& type)
		{
			LogService& log = Services::Get<LogService>();

			String full_path = FullPath(path);

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(full_path);

			if (it != map.end())
			{
				return it->second.get();
			}

			log.Log(console::LogSeverity::kError, "Content with path '{0}' could not be found\nAre you sure it has been loaded correctly and the type is correct?", path);
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		ContentBase* ContentManager::LoadContent(const String& path, const ContentBase::Types& type)
		{
			LogService& log = Services::Get<LogService>();

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(path);

			if (it != map.end())
			{
				log.Log(console::LogSeverity::kWarning, "Content with path '{0}' was already loaded, skipping load", path);
				return it->second.get();
			}

			String full_path = FullPath(path);

			ContentBase* content = nullptr;
			
			switch (type)
			{
			case ContentBase::Types::kScript:
				content = Memory::default_allocator().Construct<Script>();
				break;

			default:
				break;
			}

			log.Assert(content != nullptr, "Content to be loaded from path '{0}' with type '{1}' was null after file reading", path, type);

			File* f = File::Open(full_path, File::AccessFlags::kRead | File::AccessFlags::kBinary);
			content->Load(f);
			File::Close(f);

			SharedPtr<ContentBase> shared = Memory::MakeShared<ContentBase>(content);
			map.emplace(full_path, shared);
			
			watch_.Add(full_path);

			return shared.get();
		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::UnloadContent(const String& path, const ContentBase::Types& type)
		{
			String full_path = FullPath(path);

			LogService& log = Services::Get<LogService>();

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(full_path);

			if (it != map.end())
			{
				watch_.Remove(full_path);

				map.erase(it);
				return;
			}

			log.Log(console::LogSeverity::kWarning, "Content with path '{0}' was never loaded, skipping unload", path);
		}

		//-----------------------------------------------------------------------------------------------
		String ContentManager::FullPath(const String& path) const
		{
			return src_directory_.size() > 0 ? src_directory_ + "/" + path : path;
		}

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_SINGLE(ContentManager, JS_BODY({
			
			JSFunctionRegister funcs[] =
			{
				JS_FUNCTION_REG(load),
				JS_FUNCTION_REG(unload),
				JS_FUNCTION_REG_END
			};

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(ContentManager, load, JS_BODY({

		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(ContentManager, unload, JS_BODY({

		}));
	}
}