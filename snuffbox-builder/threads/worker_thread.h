#pragma once

#include <thread>
#include <mutex>

#include <snuffbox-compilers/compilers/script_compiler.h>

namespace snuffbox
{
	namespace builder
	{
		class BuildThread;
		class Builder;

		/**
		* @class snuffbox::builder::WorkerThread
		* @brief A worker thread compiles a single file and stores it in the binary directory
		* @author Daniel Konings
		*/
		class WorkerThread
		{

			friend class BuildThread;
			friend class Builder;

		protected:

			/**
			* @brief The different file types
			*/
			enum struct FileType
			{
				kScript, //!< A JavaScript file
				kSkip, //!< Skip this file
				kCount //!< The number of different file types
			};

			/**
			* @struct snuffbox::builder::WorkerThread::BuildCommand
			* @brief A build command to be handled by a worker thread
			* @author Daniel Konings
			*/
			struct BuildCommand
			{
				std::string src_path; //!< The source path to build from
				std::string build_path; //!< The build path to build to
				FileType file_type; //!< The file type to build
			};

			/**
			* @brief Initialises this worker thread with the current build thread
			* @param[in] build_thread (snuffbox::builder::BuildThread*) The build thread
			*/
			WorkerThread(BuildThread* build_thread);

			/**
			* @brief Resets this thread for use
			*/
			void Reset();

			/**
			* @brief Sets an error message of this worker thread
			* @param[in] error (const std::string&) The error message to set
			* @remarks This will set snuffbox::builder::WorkerThread::has_error_ to true, it will also finish the thread
			*/
			void SetError(const std::string& error);

			/**
			* @brief Runs this worker thread with a given ID
			* @param[in] id (int) The ID to assign to this worker thread
			* @param[in] cmd (const snuffbox::builder::WorkerThread::BuildCommand&) The command to execute
			*/
			void Run(int id, const BuildCommand& cmd);

			/**
			* @brief Attempts to join this thread
			*/
			void Join();

			/**
			* @brief Attempts to open a file and returns the contents
			* @param[in] path (const std::string&) The path to the file
			* @param[out] file_size (size_t*) The size of the file
			* @return (unsigned char*) The contents of the file, or nullptr if failed
			*/
			unsigned char* OpenFile(const std::string& path, size_t* file_size);

			/**
			* @brief Checks if this thread had any errors and if so, returns it
			* @param[out] has_error (bool*) Did this thread encounter an error?
			* @return (const std::string&) The error this thread has encountered, if any
			*/
			const std::string& GetError(bool* has_error) const;

			/**
			* @return (const int&) The ID of this thread
			*/
			const int& id() const;

			/**
			* @return (const bool&) Has this thread finished?
			*/
			const bool& finished() const;

		private:

			BuildThread* build_thread_; //!< The current build thread

			std::thread thread_; //!< The actual thread

			bool has_error_; //!< Does this thread have any errors?
			std::string error_; //!< The error this thread has encountered, if any

			int id_; //!< The ID of this worker thread
			BuildCommand command_; //!< The current build command of this thread

			bool finished_; //!< Has this thread finished?

			compilers::ScriptCompiler script_compiler_; //!< The compiler for scripts
		};
	}
}