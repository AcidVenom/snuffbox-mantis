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
					std::unique_lock<std::mutex> cv_lock(wait_mutex_);
					wait_cv_.wait(cv_lock);

					if (building_ == false)
					{
						break;
					}

					std::lock_guard<std::mutex> queue_lock(queue_mutex_);
					while (queue_.empty() == false && building_ == true)
					{
						const WorkerThread::BuildCommand& front = queue_.front();
						if (static_cast<unsigned int>(current_thread) < MAX_THREADS_)
						{
							threads_.at(current_thread)->Run(current_thread, front);
							FindCurrent();

							queue_.pop();

							continue;
						}

						queue_.pop();

						FindCurrent();
					}
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
		void BuildThread::LockQueue()
		{
			queue_mutex_.lock();
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::Queue(const WorkerThread::BuildCommand& cmd)
		{
			queue_.push(cmd);
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::UnlockQueue()
		{
			queue_mutex_.unlock();
			wait_cv_.notify_all();
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::OnFinish(const WorkerThread* thread)
		{
			bool has_error = false;
			const std::string& error = thread->GetError(&has_error);

			if (has_error == true)
			{
				
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