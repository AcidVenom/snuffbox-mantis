#include "content.h"
#include "../services/log_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		ContentBase::ContentBase() :
			is_valid_(false)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void ContentBase::set_is_valid(bool valid)
		{
			is_valid_ = valid;
		}

		//-----------------------------------------------------------------------------------------------
		bool ContentBase::is_valid() const
		{
			return is_valid_;
		}

		//-----------------------------------------------------------------------------------------------
		ContentBase::~ContentBase()
		{

		}
	}
}