#pragma once

#include "worker_thread.h"

#include <vector>
#include <mutex>
#include <condition_variable>

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
			* @brief Notify about a new build command that is available
			* @param[in] cmd (const snuffbox::builder::WorkerThread::BuildCommand&) The command to execute
			*/
			void Notify(const WorkerThread::BuildCommand& cmd);

			/**
			* @brief Called when a worker thread has finished its work
			* @param[in] thread (const snuffbox::builder::WorkerThread*) The worker thread that finished
			*/
			void OnFinish(const WorkerThread* thread);

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

			WorkerThread::BuildCommand command_; //!< The current build command

			std::mutex wait_mutex_; //!< The mutex to lock while waiting for input
			std::condition_variable wait_cv_; //!< The condition variable to notify when there is new input

			static const unsigned int MAX_THREADS_; //!< The maximum number of threads
		};
	}
}