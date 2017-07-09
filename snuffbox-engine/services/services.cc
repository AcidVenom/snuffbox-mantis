#include "services.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		void* Services::services_[ServiceIDs::kCount] = { 
			nullptr
		};
	}
}