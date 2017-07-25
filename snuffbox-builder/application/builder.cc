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
			"Idle"
		};

		//-----------------------------------------------------------------------------------------------
		Builder::Builder(wxWindow* parent) :
			MainWindow(parent),
			status_(BuildStatus::kStopped),
			compiled_(0),
			to_compile_(0)
		{
			button_start->Disable();
			button_stop->Disable();
			
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
			ctrl->SetValue(path);
			paths_[static_cast<int>(type)] = path;

			SetStatusText(wxString("Set ") + (type == DirectoryType::kSource ? "source" : "build") + " directory to '" + path + "'");

			if (save == true)
			{
				SaveSettings();
			}

			if (type == DirectoryType::kSource)
			{
				std::ifstream fin(path.ToStdString() + "/.snuff");

				if (fin.is_open() == true)
				{
					Log("This source directory appears to be valid");

					ListSource();
					
					button_start->Enable();

					fin.close();

					return;
				}

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

			SetStatusText("Stopped build");
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

			wxString directory_string = dir_count == 1 ? "directory" : "directories";

			Log(wxString("Found ") + std::to_string(dir_count) + " " + directory_string + " and " + std::to_string(file_count) + " file(s)");
		
			to_compile_ = file_count;

			UpdateProgress();
		}

		//-----------------------------------------------------------------------------------------------
		void Builder::UpdateProgress()
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
			output_text->AppendText(CreateTimeStamp() + " " + message + "\n");
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
		void Builder::SwitchStatus(BuildStatus status)
		{
			switch (status)
			{
			case BuildStatus::kStopped:
				Stop();
				break;

			case BuildStatus::kBuilding:
				Build();
				break;

			case BuildStatus::kIdle:
				break;

			default:
				break;
			}

			if (status != BuildStatus::kCount)
			{
				label_current->SetLabelText(STATUS_TEXTS_[static_cast<int>(status)]);
			}
		}
	}
}