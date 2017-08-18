#include "js_object.h"

using namespace v8;

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		JSObject::JSObject(const JSObject& other)
		{

		}

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