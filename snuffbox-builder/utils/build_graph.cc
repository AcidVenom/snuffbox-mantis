#include "build_graph.h"

#include <assert.h>

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		const size_t BuildGraph::BuildData::BINARY_OFFSET = sizeof(std::string);
		const size_t BuildGraph::BuildData::BINARY_SIZE = sizeof(BuildGraph::BuildData) - BuildGraph::BuildData::BINARY_OFFSET;

		//-----------------------------------------------------------------------------------------------
		BuildGraph::BuildGraph()
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int BuildGraph::Sync(const DirectoryLister* lister, const std::string& bin_path)
		{
			const DirectoryLister::DirectoryTree& tree = lister->tree();

			Graph new_graph;
			BuildData data;

			for (DirectoryLister::DirectoryTree::const_iterator it = tree.begin(); it != tree.end(); ++it)
			{
				for (int i = 0; i < static_cast<int>(it->second.size()); ++i)
				{
					data.path = it->first + "/" + it->second.at(i);
					data.is_content = data.was_build = false;
					data.last_build = data.last_modified = time(0);

					new_graph.push_back(data);
				}
			}

			unsigned int built = 0;
			bool found = false;
			for (int i = 0; i < graph_.size(); ++i)
			{
				found = false;
				for (int j = 0; j < new_graph.size(); ++j)
				{
					if (graph_.at(i).path == new_graph.at(j).path)
					{
						found = true;
						new_graph.at(j) = graph_.at(i);

						if (new_graph.at(j).was_build == true)
						{
							++built;
						}

						break;
					}
				}

				if (found == false)
				{
					remove((bin_path + "/" + graph_.at(i).path).c_str());
				}
			}

			graph_ = new_graph;

			return built;
		}

		//-----------------------------------------------------------------------------------------------
		void BuildGraph::Save(const std::string& bin) const
		{
			std::string full_path = bin + "/.build_graph";
			remove(full_path.c_str());

			std::ofstream fout(full_path, std::ios::binary);

			if (fout.is_open() == false)
			{
				return;
			}

			for (int i = 0; i < graph_.size(); ++i)
			{
				WriteToFile(fout, graph_.at(i), i == graph_.size() - 1);
			}

			fout.close();
		}

		//-----------------------------------------------------------------------------------------------
		void BuildGraph::WriteToFile(std::ofstream& file, const BuildData& data, bool last) const
		{
			size_t size = data.path.size();
			assert(size + 1 <= 256);
			
			file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
			file.write(data.path.c_str(), data.path.size());

			const char* binary = reinterpret_cast<const char*>(&data) + BuildGraph::BuildData::BINARY_OFFSET;

			file.write(binary, BuildData::BINARY_SIZE);

			if (last == false)
			{
				file.write(reinterpret_cast<char*>(&last), sizeof(bool));
			}
		}

		//-----------------------------------------------------------------------------------------------
		void BuildGraph::Load(const std::string& bin)
		{
			std::string full_path = bin + "/.build_graph";
			std::ifstream fin(full_path, std::ios::binary);

			if (fin.is_open() == false)
			{
				return;
			}

			graph_.clear();

			BuildData data;

			size_t size;
			bool eof;
			char buffer[256];

			while (fin.eof() == false)
			{
				fin.read(reinterpret_cast<char*>(&size), sizeof(size_t));

				if (fin.eof() == true)
				{
					break;
				}
				
				assert(size + 1 <= 256);
				fin.read(buffer, size);

				memset(buffer + size, '\0', sizeof(char));

				data.path = buffer;
				
				fin.read(reinterpret_cast<char*>(&data) + BuildGraph::BuildData::BINARY_OFFSET, BuildData::BINARY_SIZE);

				graph_.push_back(data);

				fin.read(reinterpret_cast<char*>(&eof), sizeof(bool));
			}

			fin.close();
		}
	}
}