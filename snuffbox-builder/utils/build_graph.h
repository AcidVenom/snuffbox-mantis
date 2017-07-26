#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#include "../platform/platform_directory_lister.h"

namespace snuffbox
{
	namespace builder
	{
		class Builder;

		/**
		* @class snuffbox::builder::BuildGraph
		* @brief Used to check the current build dependencies and act accordingly
		* @remarks e.g. Rebuilding only when necessary, even between sessions
		* @author Daniel Konings
		*/
		class BuildGraph
		{

			friend class Builder;

		protected:

			/**
			* @struct snuffbox::builder::BuildGraph::BuildData
			* @brief Build data to be stored in a file so we can check the current build between sessions
			* @remarks This structure will be loaded into memory once and then used from there
			* @author Daniel Konings
			*/
			struct BuildData
			{
				std::string path; //!< The path to the file to build
				bool is_content; //!< Is this file actual content?
				bool was_build; //!< Was the file already build before?
				tm last_modified; //!< The last time the file was modified
				tm last_build; //!< The last time the file was build

				static const size_t BINARY_OFFSET;
				static const size_t BINARY_SIZE;
			};

			/**
			* @brief Default constructor, loads any old build data
			*/
			BuildGraph();

			/**
			* @brief Syncs the build graph with the source directory lister
			* @param[in] lister (const snuffbox::builder::DirectoryLister*) The lister to sync with
			* @param[in] bin (const std::string&) The path to the binary directory
			* @return (unsigned int) How many files were already built?
			*/
			unsigned int Sync(const DirectoryLister* lister, const std::string& src, const std::string& bin);

			/**
			* @brief Saves the current build graph to the binary path
			* @param[in] bin (const std::string&) The current binary path
			*/
			void Save(const std::string& bin) const;

			/**
			* @brief Write build data to a file
			* @param[in] file (std::ofstream&) The file to write to
			* @param[in] data (const snuffbox::builder::BuildData&) The data to write
			* @param[in] last (bool) Is this the last entry?
			*/
			void WriteToFile(std::ofstream& file, const BuildData& data, bool last) const;

			/**
			* @brief Loads an old build graph from the binary path
			* @param[in] bin (const std::string&) The current binary path
			*/
			void Load(const std::string& bin);

			/**
			* @brief Retrieves the last modified time attribute from a file
			* @param[in] path (const std::string&) The path to the file
			* @return (tm) The last modified time
			*/
			static tm GetFileTime(const std::string& path);

		private:

			typedef std::vector<BuildData> Graph;

			Graph graph_; //!< The full graph of build data
		};
	}
}