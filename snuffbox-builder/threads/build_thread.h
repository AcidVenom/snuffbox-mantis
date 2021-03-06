#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "worker_thread.h"

namespace snuffbox
{
	namespace builder
	{
		class Builder;
		class BuildGraph;

		/**
		* @class snuffbox::builder::BuildThread
		* @brief The build thread that queues worker threads to do building
		* @author Daniel Konings
		*/
		class BuildThread
		{

			friend class Builder;
			friend class WorkerThread;
			friend class BuildGraph;

		protected:

			/**
			* @brief Construct through the builder
			* @param[in] builder (snuffbox::builder::Builder*) The builder this build thread should run in
			*/
			BuildThread(Builder* builder);

			/**
			* @brief Runs the build thread
			*/
			void Run();

			/**
			* @brief Stops the build thread
			*/
			void Stop();
			
			/**
			* @brief Queues a new build command that is available
			* @param[in] cmd (const snuffbox::builder::WorkerThread::BuildCommand&) The command to queue
			*/
			void Queue(const WorkerThread::BuildCommand& cmd);

			/**
			* @brief Called when a worker thread has finished its work
			* @param[in] thread (const snuffbox::builder::WorkerThread*) The worker thread that finished
			* @param[in] compiled (const std::string&) The path to the file that was compiled
			*/
			void OnCompiled(const WorkerThread* thread, const std::string& compiled);

			/**
			* @brief Called when the build has finished
			* @param[in] num_compiled (unsigned int) How many files were compiled?
			*/
			void OnFinished(unsigned int num_compiled);

		public:

			/**
			* @brief Default destructor, cleans up all threads
			*/
			~BuildThread();

		private:

			Builder* builder_; //!< The builder GUI
			bool building_; //!< Is the build thread building?

			std::thread build_thread_; //!< The actual build thread
			std::vector<WorkerThread*> threads_; //!< All current worker threads

			std::queue<WorkerThread::BuildCommand> queue_; //!< The current build commands

			std::mutex queue_mutex_; //!< The mutex for the build queue

			static const unsigned int MAX_THREADS_; //!< The maximum number of threads
		};
	}
}