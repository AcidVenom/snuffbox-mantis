#include "content_manager.h"
#include "../services/log_service.h"
#include "../logging/cvar.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		ContentManager::ContentManager()
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

				log.Log(console::LogSeverity::kInfo, "Set the source directory of the project to '{0}'", src_directory_.c_str());

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
		ContentBase* ContentManager::GetContent(const String& path, const ContentBase::Types& type)
		{
			LogService& log = Services::Get<LogService>();

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(path);

			if (it != map.end())
			{
				return it->second.get();
			}

			log.Log(console::LogSeverity::kError, "Content with path '{0}' could not be found\nAre you sure it has been loaded correctly and the type is correct?", path.c_str());
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
				log.Log(console::LogSeverity::kWarning, "Content with path '{0}' was already loaded, skipping load", path.c_str());
				return it->second.get();
			}

			ContentBase* content = nullptr;

			log.Assert(content != nullptr, "Content to be loaded from path '{0}' with type '{1}' was null after file reading", path.c_str(), type);

			SharedPtr<ContentBase> shared = Memory::MakeShared<ContentBase>(content);
			map.emplace(path, shared);
			
			return shared.get();
		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::UnloadContent(const String& path, const ContentBase::Types& type)
		{
			LogService& log = Services::Get<LogService>();

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(path);

			if (it != map.end())
			{
				map.erase(it);
				return;
			}

			log.Log(console::LogSeverity::kWarning, "Content with path '{0}' was never loaded, skipping unload", path.c_str());
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