#include "js_object.h"

using namespace v8;

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Persistent<Object>& JSObject::object()
		{
			return object_;
		}

		//-----------------------------------------------------------------------------------------------
		JSObject::~JSObject()
		{

		}
	}
}