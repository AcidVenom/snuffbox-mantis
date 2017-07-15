#include "file_watch.h"
#include "file.h"
#include "content_manager.h"
#include "../services/log_service.h"

#ifdef SNUFF_WIN32
#define localtime(out, time) { localtime_s(&out, time); }
#else
#define localtime(out, time) { out = *localtime(time); }
#endif

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		FileWatch::FileWatch(ContentManager* cm) :
			content_manager_(cm),
			frame_count_(0)
		{

		}

		//-----------------------------------------------------------------------------------------------
		tm FileWatch::GetFileTime(const String& path)
		{
			FILE* f = nullptr;

			int attempts = 0;
			while (f == nullptr && attempts < 100)
			{
				fopen(f, path.c_str(), "r");
				++attempts;
			}

			if (f == nullptr)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kWarning, "Couldn't open file '{0}' for reloading");
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

			++frame_count_;

			if (frame_count_ > SNUFF_RELOAD_AFTER)
			{
				tm last, now;
				String path;

				for (FileTimeMap::iterator& it = file_times_.begin(); it != file_times_.end(); ++it)
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

				frame_count_ = 0;
			}
		}
	}
}