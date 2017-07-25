#pragma once

#include <vector>
#include <string>
#include <map>

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

			typedef std::map<std::string, std::vector<std::string>> DirectoryTree;

			/**
			* @brief Default constructor
			*/
			Win32DirectoryLister();

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
			* @return (const snuffbox::builder::Win32DirectoryLister::DirectoryTree&) The directory tree
			*/
			const DirectoryTree& tree() const;

		protected:

			/**
			* @brief Checks if a directory exists
			* @param[in] path (const std::string&) The path to the directory
			* @return (bool) Does it exist?
			*/
			static bool DirectoryExists(const std::string& path);

		private:

			std::string root_; //!< The current root directory

			DirectoryTree tree_; //!< The directory tree
		};
	}
}