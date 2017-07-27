#include "file_watch.h"
#include "file.h"
#include "content_manager.h"

#include "../services/log_service.h"
#include "../services/cvar_service.h"

#ifdef SNUFF_WIN32
	#define localtime(out, time) { localtime_s(&out, time); }
#else
	#include <sys/stat.h>
	#define localtime(out, time) { out = *localtime(time); }
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		FileWatch::FileWatch(ContentManager* cm) :
			content_manager_(cm),
			reload_timer_("Reload timer")
		{
			reload_timer_.Start();
		}

		//-----------------------------------------------------------------------------------------------
		tm FileWatch::GetFileTime(const String& path)
		{
			FILE* f = nullptr;

			int attempts = 0;
			while (f == nullptr && attempts < 1000)
			{
				fopen(f, path.c_str(), "r");
				++attempts;
			}

			if (f == nullptr)
			{
				return file_times_[path];
			}

			struct stat attributes;
			stat(path.c_str(), &attributes);

			tm out;
			localtime(out, &attributes.st_mtime);

			fclose(f);

			return out;
		}

		//-----------------------------------------------------------------------------------------------
		bool FileWatch::Compare(tm last, tm now)
		{
			return difftime(mktime(&now), mktime(&last)) > 0.0;
		}

		//-----------------------------------------------------------------------------------------------
		void FileWatch::Add(const String& path)
		{
			file_times_.emplace(path, GetFileTime(path));
		}

		//-----------------------------------------------------------------------------------------------
		void FileWatch::Remove(const String& path)
		{
			file_times_.erase(path);
		}

		//-----------------------------------------------------------------------------------------------
		void FileWatch::Update()
		{
			if (content_manager_ == nullptr)
			{
				return;
			}

			unsigned int reload_after = SNUFF_RELOAD_AFTER;

			CVarNumber* freq = Services::Get<CVarService>().Get<CVarNumber>("reload_freq");
			if (freq != nullptr)
			{
				reload_after = freq->As<unsigned int>();
			}

			unsigned int elapsed = static_cast<unsigned int>(reload_timer_.Elapsed());
			if (elapsed > reload_after)
			{
				tm last, now;
				String path;

                for (FileTimeMap::iterator it = file_times_.begin(); it != file_times_.end(); ++it)
				{
					path = it->first;

					last = it->second;
					now = GetFileTime(path);

					if (Compare(last, now) == true)
					{
						content_manager_->Reload(path);
						it->second = now;
					}
				}

				reload_timer_.Stop();
				reload_timer_.Start(true);
			}
		}
	}
}
