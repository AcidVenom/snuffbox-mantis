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
		ContentPtr<ContentBase> ContentService::GetContent(const String& path, ContentBase::Types type, bool quiet)
		{
			return ContentPtr<ContentBase>();
		}

		//-----------------------------------------------------------------------------------------------
		ContentPtr<ContentBase> ContentService::LoadContent(const String& path, ContentBase::Types type, bool quiet)
		{
			return ContentPtr<ContentBase>();
		}

		//-----------------------------------------------------------------------------------------------
		void ContentService::UnloadContent(const String& path, ContentBase::Types type, bool quiet)
		{
			
		}

		//-----------------------------------------------------------------------------------------------
		void ContentService::UnloadAll()
		{
			
		}
	}
}