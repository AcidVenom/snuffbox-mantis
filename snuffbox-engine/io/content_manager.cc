#include "content_manager.h"
#include "file.h"

#include "../services/log_service.h"
#include "../logging/cvar.h"

#include "../application/application.h"

#include "script.h"
#include "shader.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		ContentManager::ContentManager() :
			watch_(this),
			renderer_(nullptr),
			application_(nullptr)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::Initialise(CVar* cvar, graphics::Renderer* renderer, SnuffboxApp* app)
		{
			renderer_ = renderer;
			application_ = app;

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
					it->second->Reload(f, this);
					File::Close(f);

					break;
				}
			}

			application_->Reload(path.c_str() + FullPath("").size());

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
		ContentPtr<ContentBase> ContentManager::GetContent(const String& path, ContentBase::Types type, bool quiet)
		{
			LogService& log = Services::Get<LogService>();

			String full_path = FullPath(path);

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(full_path);

			if (it != map.end())
			{
				return it->second;
			}

			if (quiet == false)
			{
				log.Log(console::LogSeverity::kError, "Content with path '{0}' could not be found\nAre you sure it has been loaded correctly and the type is correct?", path);
			}
			
			return ContentPtr<ContentBase>();
		}

		//-----------------------------------------------------------------------------------------------
		ContentPtr<ContentBase> ContentManager::LoadContent(const String& path, ContentBase::Types type, bool quiet)
		{
			LogService& log = Services::Get<LogService>();

			if (quiet == false)
			{
				log.Log(console::LogSeverity::kDebug, "Loading '{0}'", path);
			}

			String full_path = FullPath(path);

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(full_path);

			if (it != map.end())
			{
				if (quiet == false)
				{
					log.Log(console::LogSeverity::kWarning, "Content with path '{0}' was already loaded, skipping load", path);
				}

				return it->second;
			}

			ContentPtr<ContentBase> content = nullptr;
			
			switch (type)
			{
			case ContentBase::Types::kScript:
				content = Memory::default_allocator().Construct<Script>();
				break;

			case ContentBase::Types::kShader:
				content = Memory::default_allocator().Construct<Shader>();
				break;

			default:
				break;
			}

			log.Assert(content.ptr() != nullptr, "Content to be loaded from path '{0}' with type '{1}' was null after file reading", path, type);

			File* f = File::Open(full_path, File::AccessFlags::kRead | File::AccessFlags::kBinary);
			bool success = content.ptr()->Load(f, this);
			File::Close(f);

			if (success == false)
			{
				return ContentPtr<ContentBase>();
			}

			content.ptr()->set_is_valid(true);
			map.emplace(full_path, content);
			
			watch_.Add(full_path);

			if (quiet == false)
			{
				log.Log(console::LogSeverity::kDebug, "Loaded '{0}'", path);
			}

			return content;
		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::UnloadContent(const String& path, ContentBase::Types type, bool quiet)
		{
			String full_path = FullPath(path);

			LogService& log = Services::Get<LogService>();

			ContentMap& map = loaded_content_[type];
			ContentMap::iterator it = map.find(full_path);

			if (it != map.end())
			{
				watch_.Remove(full_path);
				it->second.Get()->set_is_valid(false);
				it->second.Get()->Unload(this);
				map.erase(it);
				return;
			}

			if (quiet == false)
			{
				log.Log(console::LogSeverity::kWarning, "Content with path '{0}' was never loaded, skipping unload", path);
			}
		}

		//-----------------------------------------------------------------------------------------------
		void ContentManager::UnloadAll()
		{
			for (int i = 0; i < ContentBase::Types::kCount; ++i)
			{
				ContentMap& map = loaded_content_[i];

				if (map.empty() == true)
				{
					continue;
				}

				for (ContentMap::iterator it = map.begin(); it != map.end(); ++it)
				{
					it->second->Unload(this);
					it->second->set_is_valid(false);
				}

				map.clear();
			}
		}

		//-----------------------------------------------------------------------------------------------
		String ContentManager::FullPath(const String& path) const
		{
			return src_directory_.size() > 0 ? src_directory_ + "/" + path : path;
		}

		//-----------------------------------------------------------------------------------------------
		graphics::Renderer* ContentManager::renderer() const
		{
			return renderer_;
		}

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_SINGLE(ContentManager, JS_BODY(
		{
			JSFunctionRegister funcs[] =
			{
				JS_FUNCTION_REG(load),
				JS_FUNCTION_REG(get),
				JS_FUNCTION_REG(unload),
				JS_FUNCTION_REG(unloadAll),
				JS_FUNCTION_REG_END
			};

			JSWrapper::SetObjectValue(obj, "Script", static_cast<int>(ContentBase::Types::kScript));
			JSWrapper::SetObjectValue(obj, "Shader", static_cast<int>(ContentBase::Types::kShader));

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(ContentManager, load, JS_BODY(
		{
			JSWrapper wrapper(args);

			if (wrapper.Check("SN") == true)
			{
				String path = wrapper.GetValue<String>(0, "");
				ContentBase::Types type = static_cast<ContentBase::Types>(wrapper.GetValue<int>(1, static_cast<int>(ContentBase::Types::kCount)));
				ContentService& cs = Services::Get<ContentService>();

				ContentManager& cm = static_cast<ContentManager&>(cs);

				v8::Local<v8::Object> ptr = JSWrapper::New<ContentPtr<ContentBase>>(cm.LoadContent(path, type, false));
				wrapper.ReturnValue<v8::Local<v8::Object>>(ptr);
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(ContentManager, get, JS_BODY(
		{
			JSWrapper wrapper(args);

			if (wrapper.Check("SN") == true)
			{
				String path = wrapper.GetValue<String>(0, "");
				ContentBase::Types type = static_cast<ContentBase::Types>(wrapper.GetValue<int>(1, static_cast<int>(ContentBase::Types::kCount)));
				ContentService& cs = Services::Get<ContentService>();

				ContentManager& cm = static_cast<ContentManager&>(cs);

				v8::Local<v8::Object> ptr = JSWrapper::New<ContentPtr<ContentBase>>(cm.GetContent(path, type, false));
				wrapper.ReturnValue<v8::Local<v8::Object>>(ptr);
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(ContentManager, unload, JS_BODY(
		{
			JSWrapper wrapper(args);

			if (wrapper.Check("SN") == true)
			{
				String path = wrapper.GetValue<String>(0, "");
				ContentBase::Types type = static_cast<ContentBase::Types>(wrapper.GetValue<int>(1, static_cast<int>(ContentBase::Types::kCount)));
				ContentService& cs = Services::Get<ContentService>();

				ContentManager& cm = static_cast<ContentManager&>(cs);

				cm.UnloadContent(path, type, false);
			}
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(ContentManager, unloadAll, JS_BODY(
		{
			Services::Get<ContentService>().UnloadAll();
		}));
	}
}