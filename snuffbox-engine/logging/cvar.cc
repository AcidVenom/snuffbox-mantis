#include "cvar.h"

#include "../services/log_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		CVar::CVar()
		{

		}

		//-----------------------------------------------------------------------------------------------
		void CVar::Shutdown()
		{
			for (int i = 0; i < CVarBase::CVarTypes::kCount; ++i)
			{
				cvars_[i].clear();
			}
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::LogAll()
		{
			LogService& log = Services::Get<LogService>();
			log.Log(console::LogSeverity::kInfo, "Console variables:");

			const char* name = nullptr;

			for (int i = 0; i < CVarBase::CVarTypes::kCount; ++i)
			{
				CVarMap& map = cvars_[i];
				for (CVarMap::iterator it = map.begin(); it != map.end(); ++it)
				{
					name = it->first.c_str();

					switch (i)
					{
					case CVarBase::CVarTypes::kString:
						log.Log(console::LogSeverity::kInfo, "\t{0} -> {1}", name, static_cast<CVarString*>(it->second.get())->value().c_str());
						break;

					case CVarBase::CVarTypes::kBoolean:
						log.Log(console::LogSeverity::kInfo, "\t{0} -> {1}", name, static_cast<CVarBoolean*>(it->second.get())->value());
						break;

					case CVarBase::CVarTypes::kNumber:
						log.Log(console::LogSeverity::kInfo, "\t{0} -> {1}", name, static_cast<CVarNumber*>(it->second.get())->value());
						break;
					}
				}
			}
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetString(const String& name, const String& value)
		{
			DoSet<CVarString>(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetBoolean(const String& name, const bool& value)
		{
			DoSet<CVarBoolean>(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		void CVar::SetNumber(const String& name, const float& value)
		{
			DoSet<CVarNumber>(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		CVarString* CVar::GetString(const String& name)
		{
			return Find<CVarString>(name);
		}

		//-----------------------------------------------------------------------------------------------
		CVarBoolean* CVar::GetBoolean(const String& name)
		{
			return Find<CVarBoolean>(name);
		}

		//-----------------------------------------------------------------------------------------------
		CVarNumber* CVar::GetNumber(const String& name)
		{
			return Find<CVarNumber>(name);
		}

		//-----------------------------------------------------------------------------------------------
		JS_REGISTER_IMPL_SINGLE(CVar, JS_BODY({

			JSFunctionRegister funcs[] =
			{
				JS_FUNCTION_REG(set),
				JS_FUNCTION_REG(get),
				JS_FUNCTION_REG_END
			};

			JSFunctionRegister::Register(funcs, obj);
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(CVar, set, JS_BODY({
			
			JSWrapper wrapper(args);
			CVarService& cvar = Services::Get<CVarService>();
			LogService& log = Services::Get<LogService>();

			if (wrapper.Check("S") == false)
			{
				return;
			}

			String key = wrapper.GetValue<String>(0, "");

			wrapper.set_error_checks(false);
			if (wrapper.Check("SS") == true)
			{
				cvar.Set<CVarString>(key, wrapper.GetValue<String>(1, ""));
				return;
			}
			else if (wrapper.Check("SB") == true)
			{
				cvar.Set<CVarBoolean>(key, wrapper.GetValue<bool>(1, true));
				return;
			}
			else if (wrapper.Check("SN") == true)
			{
				cvar.Set<CVarNumber>(key, wrapper.GetValue<float>(1, 0.0f));
				return;
			}

			log.Log(console::LogSeverity::kError, "Could not find an appropriate CVar type for argument 2");
		}));

		//-----------------------------------------------------------------------------------------------
		JS_FUNCTION_IMPL(CVar, get, JS_BODY({
			
			JSWrapper wrapper(args);

			if (wrapper.Check("S") == true)
			{
				String name = wrapper.GetValue<String>(0, "");
				CVarService& cvar = Services::Get<CVarService>();

				CVarString* str = cvar.Get<CVarString>(name);
				if (str != nullptr)
				{
					wrapper.ReturnValue<String>(str->value());
					return;
				}
				else
				{
					CVarBoolean* b = cvar.Get<CVarBoolean>(name);
					if (b != nullptr)
					{
						wrapper.ReturnValue<bool>(b->value());
						return;
					}
					else
					{
						CVarNumber* n = cvar.Get<CVarNumber>(name);
						if (n != nullptr)
						{
							wrapper.ReturnValue<float>(n->value());
							return;
						}
					}
				}
			}
		}));
	}
}
