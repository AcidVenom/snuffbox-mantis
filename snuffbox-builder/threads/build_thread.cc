#include "build_thread.h"

#include <assert.h>

namespace snuffbox
{
	namespace builder
	{
		//-----------------------------------------------------------------------------------------------
		const unsigned int BuildThread::MAX_THREADS_ = std::thread::hardware_concurrency();

		//-----------------------------------------------------------------------------------------------
		BuildThread::BuildThread(Builder* builder) :
			builder_(builder),
			building_(false)
		{
			assert(builder != nullptr);

			threads_.resize(MAX_THREADS_);

			for (int i = 0; i < threads_.size(); ++i)
			{
				threads_.at(i) = new WorkerThread(this);
			}
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::Run()
		{
			assert(building_ == false);

			building_ = true;

			build_thread_ = std::thread([=]()
			{
				int current_thread = 0;
				bool finished = false;

				auto FindCurrent = [&]()
				{
					for (int i = 0; i < threads_.size(); ++i)
					{
						if (threads_.at(i)->finished() == true)
						{
							current_thread = i;
							return;
						}
					}

					current_thread = MAX_THREADS_;
				};

				while (building_ == true)
				{
					std::unique_lock<std::mutex> lock(wait_mutex_);
					wait_cv_.wait(lock, [=]() { return building_ == false; });

					if (building_ == false)
					{
						break;
					}

					if (static_cast<unsigned int>(current_thread) < MAX_THREADS_)
					{
						threads_.at(current_thread)->Run(current_thread, command_);
						FindCurrent();

						continue;
					}

					FindCurrent();
				}
			});
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::Stop()
		{
			building_ = false;

			wait_cv_.notify_all();

			if (build_thread_.joinable() == true)
			{
				build_thread_.join();
			}
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::Notify(const WorkerThread::BuildCommand& cmd)
		{
			std::unique_lock<std::mutex> lock(wait_mutex_);

			command_ = cmd;
			wait_cv_.notify_one();
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::OnFinish(const WorkerThread* thread)
		{
			bool has_error = false;
			const std::string& error = thread->GetError(&has_error);

			if (has_error == true)
			{
				//Do error handling
			}
		}

		//-----------------------------------------------------------------------------------------------
		BuildThread::~BuildThread()
		{
			Stop();

			for (int i = 0; i < threads_.size(); ++i)
			{
				threads_.at(i)->Join();
				delete threads_.at(i);
				threads_.at(i) = nullptr;
			}
		}
	}
}