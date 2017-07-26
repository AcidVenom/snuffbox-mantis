#pragma once

#include "../forms/main_window.h"
#include "../threads/build_thread.h"
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
			friend class BuildGraph;

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
				kExit, //!< When the user shuts down the builder
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
			* @return (unsigned int) The number of unbuild items
			*/
			unsigned int Sync();

			/**
			* @brief Rebuild when changes are detected
			* @param[in] evt (const wxCommandEvent&) The event sent by wxWidgets
			*/
			void Rebuild(const wxCommandEvent& evt);

			/**
			* @brief Makes a specified folder a source directory by placing a '.snuff' file in it
			* @param[in] path (const wxString&) The path to folder to promote
			*/
			void MakeSourceDirectory(const wxString& path);

			/**
			* @brief Progresses the progress bar by an amount, this function is thread-safe
			* @param[in] amount (unsigned int) The amount to add as progress
			*/
			void ProgressBy(unsigned int amount);

			/**
			* @brief Updates the progress bar of the builder
			* @param[in] evt (const wxCommandEvent&) The event sent by wxWidgets
			*/
			void UpdateProgress(const wxCommandEvent& evt);

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
			* @brief Updates the status text of the builder
			* @param[in] evt (const wxCommandEvent&) The event sent by wxWidgets
			*/
			void UpdateStatus(const wxCommandEvent& evt);

			/**
			* @brief Called when a file was compiled
			* @param[in] src (const std::string&) The compiled file's source directory
			*/
			void OnCompiled(const std::string& src);

			/**
			* @brief Saves the graph
			*/
			void SaveGraph();

			/**
			* @brief Joins the idle thread if possible
			*/
			void JoinIdle();

			/**
			* @brief Saves the build graph and joins the idle thread
			*/
			void FinaliseBuild();

			/**
			* @brief Retrieves the path of a directory type
			* @param[in] type (DirectoryType) The directory type to retrieve
			* @return (const wxString&) The path of this directory type
			*/
			const wxString& GetPath(DirectoryType type) const;

			/**
			* @return (wxString) A generated time stamp of the current time point
			*/
			static wxString CreateTimeStamp();

		public:

			/**
			* @brief Saves the graph on disk if we're in a valid directory
			*/
			~Builder();

		private:

			wxFont font_; //!< The font of the output

			wxString paths_[static_cast<int>(DirectoryType::kCount)]; //!< The directories set by the source/build directory pickers
			BuildStatus status_; //!< The current status of the builder

			BuildGraph graph_; //!< The current build graph
			BuildThread build_thread_; //!< The build thread

			bool is_valid_; //!< Is the current source directory valid?

			unsigned int compiled_; //!< The number of files that are already compiled
			unsigned int to_compile_; //!< The number of files to compile

			std::mutex progress_mutex_; //!< The mutex for progress
			std::mutex status_mutex_; //!< The mutex for status switches
			std::thread idle_thread_; //!< The thread to re-sync every now and then
			
			static const wxString STATUS_TEXTS_[static_cast<int>(BuildStatus::kCount)]; //!< The different status texts per status
			static const unsigned int IDLE_SLEEP_; //!< The sleep before syncing again
		};

		wxDECLARE_EVENT(BUILDER_MESSAGE, wxCommandEvent);
		wxDECLARE_EVENT(BUILDER_REBUILD, wxCommandEvent);
		wxDECLARE_EVENT(BUILDER_PROGRESS, wxCommandEvent);
		wxDECLARE_EVENT(BUILDER_STATUS, wxCommandEvent);
	}
}