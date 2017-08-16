#include "../application/builder.h"
#include "build_graph.h"

#include <assert.h>

#ifdef SNUFF_WIN32
	#define localtime(out, time) { localtime_s(&out, time); }
#else
	#include <sys/stat.h>
	#define localtime(out, time) { out = *localtime(time); }
#endif

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
		BuildGraph::Graph BuildGraph::CreateGraph(const std::string& src, const std::string& bin)
		{
			lister_.List(src);
			lister_.CreateDirectories(bin);

			Load(bin);

			const DirectoryLister::DirectoryTree& tree = lister_.tree();

			Graph new_graph;
			BuildData data;
			std::string relative, ext;

			for (DirectoryLister::DirectoryTree::const_iterator it = tree.begin(); it != tree.end(); ++it)
			{
				for (int i = 0; i < static_cast<int>(it->second.size()); ++i)
				{
					relative = it->second.at(i);
					ext = relative.c_str() + relative.find_last_of('.');

					if (relative == ".snuff" || GetFileType(ext) == BuildData::FileType::kSkip)
					{
						continue;
					}

					data.path = (it->first == "" ? "" : (it->first + '/')) + relative;
					data.is_content = data.was_build = false;

					data.last_build = data.last_modified = GetFileTime(src + '/' + data.path);

					new_graph.push_back(data);
				}
			}

			return std::move(new_graph);
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int BuildGraph::SyncGraph(Graph& graph, const std::string& src, const std::string& bin)
		{
			std::string src_path;
			std::string bin_path;
			std::ifstream fin;

			tm last_modified;

			unsigned int built = 0;
			bool found = false;
			for (int i = 0; i < data_.size(); ++i)
			{
				found = false;
				for (int j = 0; j < graph.size(); ++j)
				{
					if (data_.at(i).path == graph.at(j).path)
					{
						found = true;
						BuildData& data = graph.at(j);
						data = data_.at(i);

						if (data.was_build == true)
						{
							src_path = src + '/' + data.path;
							bin_path = bin + '/' + data.path;

							last_modified = GetFileTime(src_path);

							fin = std::ifstream(bin_path);

							if (fin.is_open() == false || difftime(mktime(&last_modified), mktime(&data.last_build)) > 0.0)
							{
								data.last_modified = last_modified;
								data.was_build = false;
								continue;
							}

							if (fin.is_open() == true)
							{
								fin.close();
							}

							++built;
						}

						break;
					}
				}

				if (found == false)
				{
					remove((bin + "/" + data_.at(i).path).c_str());
				}
			}

			return built;
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int BuildGraph::Sync(const std::string& src, const std::string& bin)
		{
			Graph new_graph = CreateGraph(src, bin);
			unsigned int built = SyncGraph(new_graph, src, bin);

			data_ = new_graph;

			return built;
		}

		//-----------------------------------------------------------------------------------------------
		BuildGraph::CompileData BuildGraph::GetCompileData()
		{
			CompileData c;
			c.not_build = 0;
			c.to_compile = static_cast<unsigned int>(data_.size());

			for (unsigned int i = 0; i < c.to_compile; ++i)
			{
				const BuildGraph::BuildData& data = data_.at(i);

				if (data.was_build == true)
				{
					continue;
				}

				++c.not_build;
			}

			return c;
		}

		//-----------------------------------------------------------------------------------------------
		BuildGraph::CompileData BuildGraph::FillQueue(BuildThread* thread, const std::string& src, const std::string& bin)
		{
			thread->queue_mutex_.lock();

			WorkerThread::BuildCommand cmd;
			std::string relative;
			std::string ext;

			CompileData c;
			c.not_build = 0;
			c.to_compile = static_cast<unsigned int>(data_.size());

			for (unsigned int i = 0; i < c.to_compile; ++i)
			{
				const BuildGraph::BuildData& data = data_.at(i);

				if (data.was_build == true)
				{
					continue;
				}

				relative = data.path;

				++c.not_build;

				cmd.src_path = src + '/' + relative;
				cmd.build_path = bin + '/' + relative;

				ext = relative.c_str() + relative.find_last_of('.');
				cmd.file_type = GetFileType(ext);

				thread->Queue(cmd);
			}

			thread->queue_mutex_.unlock();

			return c;
		}

		//-----------------------------------------------------------------------------------------------
		void BuildGraph::OnCompiled(const std::string& relative, const std::string& bin)
		{
			for (int i = 0; i < data_.size(); ++i)
			{
				BuildGraph::BuildData& data = data_.at(i);

				if (data.path == relative)
				{
					data.was_build = true;
					data.last_build = BuildGraph::GetFileTime(bin + '/' + relative);
					break;
				}
			}
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

			for (int i = 0; i < data_.size(); ++i)
			{
				WriteToFile(fout, data_.at(i), i == data_.size() - 1);
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

			data_.clear();

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

				data_.push_back(data);

				fin.read(reinterpret_cast<char*>(&eof), sizeof(bool));
			}

			fin.close();
		}

		//-----------------------------------------------------------------------------------------------
		tm BuildGraph::GetFileTime(const std::string& path)
		{
			struct stat attributes;
			stat(path.c_str(), &attributes);

			tm out;
			localtime(out, &attributes.st_mtime);

			return out;
		}

		//-----------------------------------------------------------------------------------------------
		BuildGraph::BuildData::FileType BuildGraph::GetFileType(const std::string& ext)
		{
			if (ext == ".js")
			{
				return BuildData::FileType::kScript;
			}
			else if (ext == ".vs" || ext == ".ps" || ext == ".gs")
			{
				return BuildData::FileType::kShader;
			}

			return BuildData::FileType::kSkip;
		}
	}
}