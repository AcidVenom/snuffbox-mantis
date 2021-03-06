#include "win32_directory_lister.h"
#include <Windows.h>

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		Win32DirectoryLister::Win32DirectoryLister()
		{

		}

		//-----------------------------------------------------------------------------------------------
		bool Win32DirectoryLister::List(const std::string& start_at, bool root)
		{
			if (root == true)
			{
				Clear();
			}

			WIN32_FIND_DATAA ffd;
			HANDLE current = INVALID_HANDLE_VALUE;
			DWORD error = 0;
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

			current = FindFirstFileA((start_at + "/*").c_str(), &ffd);

			if (current == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0)
				{
					continue;
				}

				relative = start_at.c_str() + root_.size() + (root == true ? 0 : 1);

				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					List(start_at + "/" + ffd.cFileName, false);
				}
				else
				{
					if (tree_.find(relative) == tree_.end())
					{
						tree_.emplace(relative, std::vector<std::string>());
					}

					tree_[relative].push_back(ffd.cFileName);
				}
			} while (FindNextFileA(current, &ffd) != 0);

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void Win32DirectoryLister::CreateDirectories(const std::string& bin)
		{
			std::string full_path;
			std::string dir;

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
					CreateDirectoryA(full_path.c_str(), NULL);
				}
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Win32DirectoryLister::Clear()
		{
			directories_.clear();
			tree_.clear();
		}

		//-----------------------------------------------------------------------------------------------
		const Win32DirectoryLister::DirectoryTree& Win32DirectoryLister::tree() const
		{
			return tree_;
		}

		//-----------------------------------------------------------------------------------------------
		bool Win32DirectoryLister::DirectoryExists(const std::string& path)
		{
			DWORD attrib = GetFileAttributesA(path.c_str());

			return (attrib != INVALID_FILE_ATTRIBUTES && ((attrib & FILE_ATTRIBUTE_DIRECTORY) == 1));
		}
	}
}