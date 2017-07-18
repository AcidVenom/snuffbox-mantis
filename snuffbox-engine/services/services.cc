#include "services.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void* Services::services_[ServiceIDs::kCount] = { 
			nullptr
		};

		//-----------------------------------------------------------------------------------------------
		std::recursive_mutex Services::service_mutex_[ServiceIDs::kCount];
	}
}