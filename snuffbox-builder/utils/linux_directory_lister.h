#pragma once

#include <map>
#include <string>
#include <vector>

namespace snuffbox
{
	namespace builder
	{
		/**
		* @class snuffbox::builder::LinuxDirectoryLister
		* @brief The directory lister for the linux platform
		* @author Daniel Konings
		*/
		class LinuxDirectoryLister
		{

		public:

			typedef std::map<std::string, std::vector<std::string>> DirectoryTree;

			/**
			* @brief Default constructor
			*/
			LinuxDirectoryLister();

			/**
			* @brief Start listing all directories and files starting at a root
			* @param[in] start_at (const std::string&) The root directory to start at
			* @param[in] root (bool) Is this the root directory? Default = true
			* @return (bool) Was listing the directories a success?
			*/
			bool List(const std::string& start_at, bool root = true);

			/**
			* @brief Create the directories that reflect on the source directory in the build directory
			* @param[in] bin (const std::string&) The build directory
			*/
			void CreateDirectories(const std::string& bin);

			/**
			* @brief Clears any old data
			*/
			void Clear();

			/**
			* @return (const snuffbox::builder::LinuxDirectoryLister::DirectoryTree&) The directory tree
			*/
			const DirectoryTree& tree() const;

		protected:

			/**
			* @brief Checks if a directory exists
			* @param[in] path (const std::string&) The path to the directory
			* @return (bool) Does it exist?
			*/
			static bool DirectoryExists(const std::string& path);

			/**
			* @brief Checks if a path is a directory
			* @param[in] path (const std::string&) The path to the file or directory
			* @return (bool) Is this path a directory?
			*/
			static bool IsDirectory(const std::string& path);

		private:

			std::string root_; //!< The current root directory

			DirectoryTree tree_; //!< The directory tree
			std::vector<std::string> directories_; //!< The directories in chronological order of recursion
		};
	}
}
