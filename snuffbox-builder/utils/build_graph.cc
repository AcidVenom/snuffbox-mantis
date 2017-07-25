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
			Load();
		}

		//-----------------------------------------------------------------------------------------------
		void BuildGraph::Save() const
		{
			remove(".build_graph");

			std::ofstream fout(".build_graph", std::ios::binary);

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
		void BuildGraph::Load()
		{
			std::ifstream fin(".build_graph", std::ios::binary);

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