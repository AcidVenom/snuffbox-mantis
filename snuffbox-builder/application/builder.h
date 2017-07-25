#pragma once

#include "../forms/main_window.h"
#include "../threads/build_thread.h"
#include "../platform/platform_directory_lister.h"
#include "../utils/build_graph.h"

namespace snuffbox
{
	namespace builder
	{
		class BuilderApp;

		/**
		* @class snuffbox::builder::Builder : public snuffbox::MainWindow
		* @brief The main builder window that logs its status and in which you can specify the settings
		* @author Daniel Konings
		*/
		class Builder : public MainWindow
		{

			friend class BuilderApp;
			friend class BuildThread;

		protected:

			/**
			* @brief The different directory types
			*/
			enum struct DirectoryType
			{
				kSource, //!< The source directory
				kBuild, //!< The build directory
				kCount //!< The total number of directory types
			};

			/**
			* @brief The different build statusses
			*/
			enum struct BuildStatus
			{
				kStopped, //!< When the builder is stopped
				kBuilding, //!< When the builder is building
				kIdle, //!< When the builder is finished but still running
				kCount //!< The number of statusses
			};

			/**
			* @brief Construct through specifying the window parent
			* @param[in] parent (wxWindow*) The parent of this window
			*/
			Builder(wxWindow* parent);

			/**
			* @brief Change the string value of the corresponding directory type when a directory was changed through a directory picker
			* @param[in] ctrl (wxTextCtrl*) The text control to change the string value of
			* @param[in] path (const wxString&) The new path
			* @param[in] type (snuffbox::builder::Builder::DirectoryType) The type of directory that was changed
			* @param[in] save (bool) Should the paths be saved in the settings?
			*/
			void ChangeDirectory(wxTextCtrl* ctrl, const wxString& path, DirectoryType type, bool save = true);

			/**
			* @brief Called when either of the directory pickers changed
			* @param[in] evt (wxFileDirPickerEvent&) The event data
			*/
			void OnDirectoryChanged(wxFileDirPickerEvent& evt);

			/**
			* @brief Called when any button is pressed
			* @param[in] evt (wxEvent&) The button event
			*/
			void OnButton(wxEvent& evt);

			/**
			* @brief Start building
			*/
			void Build();

			/**
			* @brief Stop building
			*/
			void Stop();

			/**
			* @brief Stop building but keep running
			*/
			void Idle();

			/**
			* @brief Relist all directories and synchronise the build graph with it
			*/
			void Sync();

			/**
			* @brief Makes a specified folder a source directory by placing a '.snuff' file in it
			* @param[in] path (const wxString&) The path to folder to promote
			*/
			void MakeSourceDirectory(const wxString& path);

			/**
			* @brief List the source directory
			*/
			void ListSource();

			/**
			* @brief Updates the progress bar of the builder
			*/
			void UpdateProgress();

			/**
			* @brief Save all the current settings
			*/
			void SaveSettings() const;

			/**
			* @brief Loads any old settings
			*/
			void LoadSettings();

			/**
			* @brief Logs a message with a timestamp and a newline character
			* @param[in] message (const wxString&) The message to log
			*/
			void Log(const wxString& message);

			/**
			* @brief Actually adds the line to the output after thread-to-thread data transfer
			* @param[in] evt (const wxCommandEvent& evt) The event received from wxWidgets
			*/
			void AddLine(const wxCommandEvent& evt);

			/**
			* @brief Switches the current build status
			* @param[in] status (snuffbox::builder::Builder::BuildStatus) The build status to switch to
			*/
			void SwitchStatus(BuildStatus status);

			/**
			* @return (wxString) A generated time stamp of the current time point
			*/
			static wxString CreateTimeStamp();

			/**
			* @brief Retrieves a file type from a file extension
			* @param[in] ext (const std::string&) The file extension as a string
			* @return (snuffbox::builder::WorkerThread::FileType) The file type
			*/
			static WorkerThread::FileType GetFileType(const std::string& ext);

		public:

			/**
			* @brief Saves the graph on disk if we're in a valid directory
			*/
			~Builder();

		private:

			wxFont font_; //!< The font of the output

			wxString paths_[static_cast<int>(DirectoryType::kCount)]; //!< The directories set by the source/build directory pickers
			BuildStatus status_; //!< The current status of the builder

			DirectoryLister lister_; //!< The directory lister
			BuildGraph graph_; //!< The current build graph
			BuildThread build_thread_; //!< The build thread

			bool is_valid_; //!< Is the current source directory valid?

			unsigned int compiled_; //!< The number of files that are already compiled
			unsigned int to_compile_; //!< The number of files to compile

			static const wxString STATUS_TEXTS_[static_cast<int>(BuildStatus::kCount)]; //!< The different status texts per status
		};

		wxDECLARE_EVENT(BUILDER_MESSAGE, wxCommandEvent);
	}
}