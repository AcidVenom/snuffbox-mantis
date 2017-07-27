#include "content_service.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		ContentService::ContentService()
		{

		}

		//-----------------------------------------------------------------------------------------------
		ContentBase* ContentService::GetContent(const String& path, ContentBase::Types type, bool quiet)
		{
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		ContentBase* ContentService::LoadContent(const String& path, ContentBase::Types type, bool quiet)
		{
			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		void ContentService::UnloadContent(const String& path, ContentBase::Types type, bool quiet)
		{
			
		}
	}
}