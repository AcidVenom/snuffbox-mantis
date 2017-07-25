#pragma once

#include <vector>
#include <string>

namespace snuffbox
{
	namespace builder
	{
		/**
		* @class snuffbox::builder::Win32DirectoryLister
		* @brief The directory lister for the Win32 platform
		* @author Daniel Konings
		*/
		class Win32DirectoryLister
		{

		public:

			Win32DirectoryLister();

		private:

			unsigned int folder_count_; //!< How many folders were found?
			unsigned int file_count_; //!< How many files were found?

			std::vector<std::string> folders_; //!< All folders
			std::vector<std::string> files_; //!< All files
		};
	}
}