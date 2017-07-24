#pragma once

#include "../forms/main_window.h"

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
			* @brief Makes a specified folder a source directory by placing a '.snuff' file in it
			* @param[in] path (const wxString&) The path to folder to promote
			*/
			void MakeSourceDirectory(const wxString& path);

			/**
			* @brief Save all the current settings
			*/
			void SaveSettings();

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
			* @return (wxString) A generated time stamp of the current time point
			*/
			static wxString CreateTimeStamp();

			/**
			* @brief Switches the current build status
			* @param[in] status (snuffbox::builder::Builder::BuildStatus) The build status to switch to
			*/
			void SwitchStatus(BuildStatus status);

		private:

			wxFont font_; //!< The font of the output

			wxString paths_[static_cast<int>(DirectoryType::kCount)]; //!< The directories set by the source/build directory pickers
			BuildStatus status_; //!< The current status of the builder

			static const wxString STATUS_TEXTS_[static_cast<int>(BuildStatus::kCount)]; //!< The different status texts per status
		};
	}
}