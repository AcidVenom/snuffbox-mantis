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

			threads_.resize(MAX_THREADS_, new WorkerThread(this));
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::Run()
		{
			assert(building_ == false);

			building_ = true;
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
				wait_cv_.wait(lock);

				if (static_cast<unsigned int>(current_thread) < MAX_THREADS_)
				{
					threads_.at(current_thread)->Run(current_thread, command_);
					FindCurrent();

					continue;
				}

				FindCurrent();
			}
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
			for (int i = 0; i < threads_.size(); ++i)
			{
				threads_.at(i)->Join();
				delete threads_.at(i);
				threads_.at(i) = nullptr;
			}
		}
	}
}