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
		class BuildThread;

		/**
		* @class snuffbox::builder::BuildGraph
		* @brief Used to check the current build dependencies and act accordingly
		* @remarks e.g. Rebuilding only when necessary, even between sessions
		* @author Daniel Konings
		*/
		class BuildGraph
		{

			friend class Builder;

		public:

			/**
			* @struct snuffbox::builder::BuildGraph::BuildData
			* @brief Build data to be stored in a file so we can check the current build between sessions
			* @remarks This structure will be loaded into memory once and then used from there
			* @author Daniel Konings
			*/
			struct BuildData
			{
				/**
				* @brief The different file types
				*/
				enum struct FileType
				{
					kScript, //!< A JavaScript file
					kShader, //!< A shader file
					kSkip //!< An unknown file to skip
				};

				std::string path; //!< The path to the file to build
				bool is_content; //!< Is this file actual content?
				bool was_build; //!< Was the file already build before?
				tm last_modified; //!< The last time the file was modified
				tm last_build; //!< The last time the file was build

				static const size_t BINARY_OFFSET;
				static const size_t BINARY_SIZE;
			};

			/**
			* @struct snuffbox::builder::BuildGraph::CompileData
			* @brief Compile data containing how many files have to be build and how many are already built
			* @author Daniel Konings
			*/
			struct CompileData
			{
				unsigned int not_build; //!< How many files were not built yet
				unsigned int to_compile; //!< How many files are there in total to build
			};

		protected:

			typedef std::vector<BuildData> Graph;

			/**
			* @brief Default constructor, loads any old build data
			*/
			BuildGraph();

			/**
			* @brief Creates a new graph from the lister tree
			* @param[in] src (const std::string&) The current source directory
			* @param[in] bin (const std::string&) The current build directory
			* @remarks This method also relists the source directory using the directory lister
			* @return (snuffbox::builder::BuildGraph::Graph) The newly created graph
			*/
			Graph CreateGraph(const std::string& src, const std::string& bin);

			/**
			* @brief Synchronises the a graph based on the current graph
			* @param[in] graph (snuffbox::builder::BuildGraph::Graph&) The graph to synchronise with the current graph
			* @param[in] src (const std::string&) The current source directory
			* @param[in] bin (const std::string&) The current build directory
			* @return (unsigned int) How many files in the graph were already built?
			*/
			unsigned int SyncGraph(Graph& graph, const std::string& src, const std::string& bin);

			/**
			* @brief Syncs the build graph with the source directory lister
			* @param[in] bin (const std::string&) The path to the binary directory
			* @return (unsigned int) How many files were already built?
			*/
			unsigned int Sync(const std::string& src, const std::string& bin);

			/**
			* @brief Retrieves the compile data for the current list of files in the graph
			* @remarks This is how many files have already been build and how many there are to build in total
			* @return (snuffbox::builder::BuildGraph::CompileData) The compile data
			*/
			BuildGraph::CompileData GetCompileData();

			/**
			* @brief Fills up the build thread queue with build data
			* @param[in] thread (snuffbox::builder::BuildThread*) The thread to fill the queue of
			* @param[in] src (const std::string&) The source directory
			* @param[in] bin (const std::string&) The build directory
			* @return (snuffbox::builder::BuildGraph::CompileData) The compilation data
			* @see snuffbox::builder::BuildGraph::GetCompileData
			*/
			BuildGraph::CompileData FillQueue(BuildThread* thread, const std::string& src, const std::string& bin);

			/**
			* @brief Called when a file is compiled, the build graph will adjust the time values and set it to a built state
			* @param[in] relative (const std::string&) The relative path of the file
			* @param[in] bin (const std::string&) The binary folder
			*/
			void OnCompiled(const std::string& relative, const std::string& bin);

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

			/**
			* @brief Retrieves a file type from a file extension
			* @param[in] ext (const std::string&) The file extension as a string
			* @return (snuffbox::builder::BuildGraph::BuildData::FileType) The file type
			*/
			static BuildData::FileType GetFileType(const std::string& ext);

		private:

			Graph data_; //!< The full graph of build data
			DirectoryLister lister_; //!< The directory lister
		};
	}
}