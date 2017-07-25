#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <ctime>

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
				std::string path;
				bool is_content;
				time_t last_modified;
				time_t last_build;

				static const size_t BINARY_OFFSET;
				static const size_t BINARY_SIZE;
			};

			/**
			* @brief Default constructor, loads any old build data
			*/
			BuildGraph();

			/**
			* @brief Saves the current build graph
			* @param[in] src (const std::string&) The current source path
			*/
			void Save(const std::string& src) const;

			/**
			* @brief Write build data to a file
			* @param[in] file (std::ofstream&) The file to write to
			* @param[in] data (const snuffbox::builder::BuildData&) The data to write
			* @param[in] last (bool) Is this the last entry?
			*/
			void WriteToFile(std::ofstream& file, const BuildData& data, bool last) const;

			/**
			* @brief Loads an old build graph
			* @param[in] src (const std::string&) The current source path
			*/
			void Load(const std::string& src);

		private:

			typedef std::vector<BuildData> Graph;

			Graph graph_; //!< The full graph of build data
		};
	}
}