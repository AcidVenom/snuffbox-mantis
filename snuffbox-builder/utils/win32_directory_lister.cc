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
				root_ = start_at + "/";
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

				new_dir = start_at + "/";
				relative = new_dir.c_str() + root_.size();

				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					List(new_dir + ffd.cFileName, false);
				}
				else
				{
					if (relative.size() == 0)
					{
						relative = "./";
					}

					if (tree_.find(relative) == tree_.end())
					{
						tree_.emplace(relative, std::vector<std::string>());
					}

					tree_[relative].push_back((start_at + "/" + ffd.cFileName).c_str() + root_.size());
				}
			} while (FindNextFileA(current, &ffd) != 0);

			return true;
		}

		//-----------------------------------------------------------------------------------------------
		void Win32DirectoryLister::Clear()
		{
			tree_.clear();
		}

		//-----------------------------------------------------------------------------------------------
		const Win32DirectoryLister::DirectoryTree& Win32DirectoryLister::tree() const
		{
			return tree_;
		}
	}
}