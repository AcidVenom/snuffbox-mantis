#include "linux_directory_lister.h"
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		LinuxDirectoryLister::LinuxDirectoryLister()
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool LinuxDirectoryLister::List(const std::string& start_at, bool root)
		{
			if (root == true)
			{
				Clear();
			}

			struct dirent* entry;
			DIR* dp;

			std::string new_dir;
			std::string relative;

			if (root == true)
			{
				root_ = start_at;
			}
			else
			{
				directories_.push_back(start_at.c_str() + root_.size() + 1);
			}

			dp = opendir(start_at.c_str());

			if (dp == nullptr)
			{
				return false;
			}

			entry = readdir(dp);

			do
			{
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				{
					continue;
				}

				relative = start_at.c_str() + root_.size() + (root == true ? 0 : 1);

				if (IsDirectory(start_at + "/" + entry->d_name) == true)
				{
					List(start_at + "/" + entry->d_name, false);
				}
				else
				{
					if (tree_.find(relative) == tree_.end())
					{
						tree_.emplace(relative, std::vector<std::string>());
					}

					tree_[relative].push_back(entry->d_name);
				}
			} while ((entry = readdir(dp)) != nullptr);

			closedir(dp);

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void LinuxDirectoryLister::CreateDirectories(const std::string& bin)
		{
			std::string full_path;
			std::string dir;

			DIR* dp;

			for (int i = 0; i < directories_.size(); ++i)
			{
				dir = directories_.at(i);
				if (dir == "")
				{
					continue;
				}

				full_path = bin + '/' + dir;
				if (DirectoryExists(full_path) == false)
				{
					mkdir(full_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
				}
			}
		}

		//-----------------------------------------------------------------------------------------------
		void LinuxDirectoryLister::Clear()
		{
			directories_.clear();
			tree_.clear();
		}

		//-----------------------------------------------------------------------------------------------
		const LinuxDirectoryLister::DirectoryTree& LinuxDirectoryLister::tree() const
		{
			return tree_;
		}

		//-----------------------------------------------------------------------------------------------
		bool LinuxDirectoryLister::DirectoryExists(const std::string& path)
		{
			DIR* dp = opendir(path.c_str());
			bool exists = dp != nullptr;

			if (exists == true)
			{
				closedir(dp);
			}

			return exists;
		}

		//-----------------------------------------------------------------------------------------------
		bool LinuxDirectoryLister::IsDirectory(const std::string& path)
		{
			struct stat attrib;
			stat(path.c_str(), &attrib);
			return S_ISREG(attrib.st_mode) == 0;
		}
	}
}
