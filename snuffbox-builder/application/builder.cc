#include "builder.h"

#include <chrono>
#include <ctime>
#include <fstream>

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		const wxString Builder::STATUS_TEXTS_[static_cast<int>(BuildStatus::kCount)] = 
		{
			"Stopped",
			"Building",
			"Idle",
			"Exit"
		};

		const unsigned int Builder::IDLE_SLEEP_ = 500;

		//-----------------------------------------------------------------------------------------------
		Builder::Builder(wxWindow* parent) :
			MainWindow(parent),
			status_(BuildStatus::kStopped),
			build_thread_(this),
			compiled_(0),
			is_valid_(false),
			to_compile_(0)
		{
			Bind(BUILDER_MESSAGE, &Builder::AddLine, this);
			Bind(BUILDER_REBUILD, &Builder::Rebuild, this);
			Bind(BUILDER_PROGRESS, &Builder::UpdateProgress, this);
			Bind(BUILDER_STATUS, &Builder::UpdateStatus, this);

			button_start->Disable();
			button_stop->Disable();

			Log("Using " + std::to_string(BuildThread::MAX_THREADS_) + " worker threads");
			
			SetStatusText("Please specify a valid source/build directory");

			for (int i = 0; i < static_cast<int>(DirectoryType::kCount); ++i)
			{
				paths_[i] = "";
			}

			font_ = wxFont(10, wxFontFamily::wxFONTFAMILY_DEFAULT, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
#ifdef SNUFF_WIN32
			font_.SetFaceName("Consolas");
#elif SNUFF_LINUX
			font_.SetFaceName("Ubuntu Mono");
#endif
			output_text->SetFont(font_);

			LoadSettings();
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::ChangeDirectory(wxTextCtrl* ctrl, const wxString& path, DirectoryType type, bool save)
		{
			wxString remove_backslashes;
			remove_backslashes.resize(path.size());
			for (int i = 0; i < path.size(); ++i)
			{
				if (path.at(i) == '\\')
				{
					remove_backslashes.at(i) = '/';
					continue;
				}

				remove_backslashes.at(i) = path.at(i);
			}

			ctrl->SetValue(remove_backslashes);
			paths_[static_cast<int>(type)] = remove_backslashes;

			SetStatusText(wxString("Set ") + (type == DirectoryType::kSource ? "source" : "build") + " directory to '" + remove_backslashes + "'");

			if (save == true)
			{
				SaveSettings();
			}

			if (type == DirectoryType::kSource)
			{
				std::ifstream fin(remove_backslashes.ToStdString() + "/.snuff");

				if (fin.is_open() == true)
				{
					Log("This source directory appears to be valid");
					is_valid_ = true;
					
					button_start->Enable();

					fin.close();

					return;
				}

				is_valid_ = false;
				Log("An invalid source directory was specified, please make sure the folder contains a '.snuff' file");
				button_start->Disable();
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::OnDirectoryChanged(wxFileDirPickerEvent& evt)
		{
			wxGenericDirButton* button = static_cast<wxGenericDirButton*>(evt.GetEventObject());
			
			switch (button->GetParent()->GetId())
			{
			case DIR_PICKER_SOURCE_ID:
				ChangeDirectory(text_source_path, dir_picker_source->GetPath(), DirectoryType::kSource);
				break;

			case DIR_PICKER_BUILD_ID:
				ChangeDirectory(text_build_path, dir_picker_build->GetPath(), DirectoryType::kBuild);
				break;

			case DIR_PICKER_SNUFF_ID:
				MakeSourceDirectory(dir_picker_snuff->GetPath());
				ChangeDirectory(text_source_path, dir_picker_snuff->GetPath(), DirectoryType::kSource);
				break;

			default:
				break;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::OnButton(wxEvent& evt)
		{
			switch (evt.GetId())
			{
			case BUTTON_START_ID:
				SwitchStatus(BuildStatus::kBuilding);
				break;

			case BUTTON_STOP_ID:
				SwitchStatus(BuildStatus::kStopped);
				break;

			default:
				break;
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::Build()
		{
			dir_picker_source->Disable();
			dir_picker_build->Disable();
			dir_picker_snuff->Disable();
			button_start->Disable();
			button_stop->Enable();

			Sync();
			build_thread_.Run();

			SetStatusText("Build started..");
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::Stop()
		{
			dir_picker_source->Enable();
			dir_picker_build->Enable();
			dir_picker_snuff->Enable();
			button_stop->Disable();
			button_start->Enable();

			build_thread_.Stop();

			if (idle_thread_.joinable() == true)
			{
				idle_thread_.join();
			}

			graph_.Save(paths_[static_cast<int>(DirectoryType::kBuild)].ToStdString());
			SetStatusText("Stopped build");
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::Idle()
		{
			graph_.Save(paths_[static_cast<int>(DirectoryType::kBuild)].ToStdString());

			if (idle_thread_.joinable() == true)
			{
				idle_thread_.join();
			}

			if (build_thread_.building_ == false)
			{
				wxCommandEvent evt(BUILDER_REBUILD);
				evt.SetString("stop");
				wxPostEvent(this, evt);
				return;
			}

			idle_thread_ = std::thread([=]()
			{
				int count = 0;
				while (status_ == BuildStatus::kIdle && count == 0)
				{
					count = Sync();

					std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_SLEEP_));
				}

				if (count > 0)
				{
					wxCommandEvent evt(BUILDER_REBUILD);
					evt.SetString("build");
					wxPostEvent(this, evt);
				}
			});
		}

		//-----------------------------------------------------------------------------------------------
		unsigned int Builder::Sync()
		{
			ListSource();

			std::string src_path = paths_[static_cast<int>(DirectoryType::kSource)].ToStdString();
			std::string build_path = paths_[static_cast<int>(DirectoryType::kBuild)].ToStdString();

			lister_.CreateDirectories(build_path);

			graph_.Load(build_path);
			
			progress_mutex_.lock();
			compiled_ = graph_.Sync(&lister_, src_path, build_path);
			progress_mutex_.unlock();

			build_thread_.queue_mutex_.lock();
			
			WorkerThread::BuildCommand cmd;
			std::string relative;
			std::string ext;

			unsigned int count = 0;
			for (int i = 0; i < graph_.graph_.size(); ++i)
			{
				const BuildGraph::BuildData& data = graph_.graph_.at(i);
				
				if (data.was_build == true)
				{
					continue;
				}

				relative = data.path;

				if (relative == ".snuff")
				{
					continue;
				}

				++count;

				if (status_ == BuildStatus::kBuilding)
				{
					cmd.src_path = src_path + '/' + relative;
					cmd.build_path = build_path + '/' + relative;

					ext = relative.c_str() + relative.find_last_of('.');
					cmd.file_type = GetFileType(ext);

					build_thread_.Queue(cmd);
				}
			}

			build_thread_.queue_mutex_.unlock();

			ProgressBy(0);

			return count;
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::Rebuild(const wxCommandEvent& evt)
		{
			wxString str = evt.GetString();

			if (str == "stop")
			{
				SwitchStatus(BuildStatus::kStopped);
				SetStatusText("Check the output for errors");
				return;
			}

			build_thread_.Stop();
			SwitchStatus(BuildStatus::kBuilding);
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::MakeSourceDirectory(const wxString& path)
		{
			std::ofstream fout(path.ToStdString() + "/.snuff");
			fout << "snuffbox-mantis Source directory";
			fout.close();
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::ListSource()
		{
			lister_.List(paths_[static_cast<int>(DirectoryType::kSource)].ToStdString());
			const DirectoryLister::DirectoryTree& tree = lister_.tree();

			unsigned int dir_count = 0;

			if (tree.size() >= 1)
			{
				dir_count = static_cast<unsigned int>(tree.size()) - 1;
			}

			unsigned int file_count = 0;
			for (DirectoryLister::DirectoryTree::const_iterator it = tree.begin(); it != tree.end(); ++it)
			{
				file_count += static_cast<unsigned int>(it->second.size());
			}

			file_count -= 1;
			to_compile_ = file_count;
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::ProgressBy(unsigned int amount)
		{
			std::lock_guard<std::mutex> lock(progress_mutex_);
			compiled_ += amount;

			wxCommandEvent evt(BUILDER_PROGRESS);
			wxPostEvent(this, evt);
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::UpdateProgress(const wxCommandEvent& evt)
		{
			float percent = to_compile_ == 0 ? 1.0f : static_cast<float>(compiled_) / static_cast<float>(to_compile_);
			percent = std::fmaxf(0.0f, std::fminf(percent, 1.0f));

			gauge_progress->SetValue(static_cast<int>(std::floorf(percent * static_cast<float>(gauge_progress->GetRange()) + 0.5f)));
			label_count->SetLabelText(std::to_string(compiled_) + "/" + std::to_string(to_compile_));
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::SaveSettings() const
		{
			remove(".build_settings");
			std::ofstream fout(".build_settings");

			fout << paths_[static_cast<int>(DirectoryType::kSource)] << std::endl;
			fout << paths_[static_cast<int>(DirectoryType::kBuild)] << std::endl;

			fout.close();
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::LoadSettings()
		{
			std::ifstream fin(".build_settings");

			char src[256], bin[256];

			if (fin.is_open() == true)
			{
				Log("Loading settings from '.build_settings'");

				fin.getline(src, 256);
				fin.getline(bin, 256);

				ChangeDirectory(text_source_path, src, DirectoryType::kSource, false);
				ChangeDirectory(text_build_path, bin, DirectoryType::kBuild, false);

				fin.close();

				return;
			}

			Log("Found no old settings");
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::Log(const wxString& message)
		{
			wxCommandEvent evt(BUILDER_MESSAGE, 0);
			evt.SetString(CreateTimeStamp() + " " + message + "\n");

			wxPostEvent(this, evt);
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::AddLine(const wxCommandEvent& evt)
		{
			output_text->AppendText(evt.GetString());
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::SwitchStatus(BuildStatus status)
		{
			std::lock_guard<std::mutex> lock(status_mutex_);

			status_ = status;

			switch (status)
			{
			case BuildStatus::kStopped:
				Stop();
				break;

			case BuildStatus::kBuilding:
				Build();
				break;

			case BuildStatus::kIdle:
				Idle();
				break;

			default:
				break;
			}

			if (status != BuildStatus::kCount)
			{
				wxCommandEvent evt(BUILDER_STATUS);
				evt.SetString(STATUS_TEXTS_[static_cast<int>(status)]);

				wxPostEvent(this, evt);
			}
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::UpdateStatus(const wxCommandEvent& evt)
		{
			label_current->SetLabelText(evt.GetString());
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::OnCompiled(const std::string& path)
		{
			std::string relative = path.c_str() + paths_[static_cast<int>(DirectoryType::kSource)].size() + 1;
			std::string build_path = paths_[static_cast<int>(DirectoryType::kBuild)] + '/' + relative;

			for (int i = 0; i < graph_.graph_.size(); ++i)
			{
				BuildGraph::BuildData& data = graph_.graph_.at(i);

				if (data.path == relative)
				{
					data.was_build = true;
					data.last_build = BuildGraph::GetFileTime(build_path);
					break;
				}
			}

			ProgressBy(1);
		}

		//-----------------------------------------------------------------------------------------------
		wxString Builder::CreateTimeStamp()
		{
			std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
			std::time_t now = std::chrono::system_clock::to_time_t(tp);
			tm* time = std::localtime(&now);

			auto FormatTime = [](int time)
			{
				wxString formatted = std::to_string(time);
				formatted = formatted.size() == 1 ? "0" + formatted : formatted;

				return formatted;
			};

			return "[" + FormatTime(time->tm_hour) + ":" + FormatTime(time->tm_min) + ":" + FormatTime(time->tm_sec) + "]";
		}

		//-----------------------------------------------------------------------------------------------
		WorkerThread::FileType Builder::GetFileType(const std::string& ext)
		{
			if (ext == ".js")
			{
				return WorkerThread::FileType::kScript;
			}

			return WorkerThread::FileType::kSkip;
		}

		//-----------------------------------------------------------------------------------------------
		Builder::~Builder()
		{
			status_ = BuildStatus::kExit;
			if (is_valid_ == true)
			{
				graph_.Save(paths_[static_cast<int>(DirectoryType::kBuild)].ToStdString());
			}

			if (idle_thread_.joinable() == true)
			{
				idle_thread_.join();
			}
		}

		//-----------------------------------------------------------------------------------------------
		wxDEFINE_EVENT(BUILDER_MESSAGE, wxCommandEvent);
		wxDEFINE_EVENT(BUILDER_REBUILD, wxCommandEvent);
		wxDEFINE_EVENT(BUILDER_PROGRESS, wxCommandEvent);
		wxDEFINE_EVENT(BUILDER_STATUS, wxCommandEvent);
	}
}