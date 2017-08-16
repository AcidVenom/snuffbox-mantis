#include "../application/builder.h"

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
				unsigned int to_compile = static_cast<unsigned int>(queue_.size());
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

				queue_mutex_.lock();

				while (queue_.empty() == false && building_ == true)
				{
					const WorkerThread::BuildCommand& front = queue_.front();

					if (static_cast<unsigned int>(current_thread) < MAX_THREADS_)
					{
						builder_->Log(std::to_string(current_thread + 1) + "> " + front.src_path);
						threads_.at(current_thread)->Run(current_thread, front);
						FindCurrent();

						queue_.pop();

						continue;
					}

					FindCurrent();
				}

				if (building_ == false && queue_.size() > 0)
				{
					while (queue_.empty() == false)
					{
						queue_.pop();
					}
				}

				queue_mutex_.unlock();

				for (int i = 0; i < threads_.size(); ++i)
				{
					threads_.at(i)->Join();
				}

				OnFinished(to_compile);
			});
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::Stop()
		{
			building_ = false;

			if (build_thread_.joinable() == true)
			{
				build_thread_.join();
			}
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::Queue(const WorkerThread::BuildCommand& cmd)
		{
			queue_.push(cmd);
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::OnCompiled(const WorkerThread* thread, const std::string& compiled)
		{
			bool has_error = false;
			const std::string& error = thread->GetError(&has_error);
			std::string id = std::to_string(thread->id() + 1) + ">";

			if (has_error == true)
			{
				builder_->Log(id + " -- [ERROR] " + compiled + ": " + error.c_str());
				building_ = false;

				return;
			}

			builder_->Log(id + " -- Compiled " + compiled);
			builder_->OnCompiled(compiled);
		}

		//-----------------------------------------------------------------------------------------------
		void BuildThread::OnFinished(unsigned int num_compiled)
		{
			builder_->Log(building_ == true ? 
				"Done compiling " + std::to_string(num_compiled) + " file(s)" : "Error occurred, aborting",
				true,
				building_ == false);

			builder_->SwitchStatus(Builder::BuildStatus::kIdle);
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