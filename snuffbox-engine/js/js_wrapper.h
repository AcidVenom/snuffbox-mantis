#pragma once

#include <string>

#include "../js/js_function_register.h"
#include "../js/js_state_wrapper.h"

#include "../core/eastl.h"

#define JS_SETUP(type) JSWrapper wrapper(args); type* self = wrapper.GetPointer<type>(args.This())

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::JSWrapper
		* @brief A wrapper around v8::FunctionCallbackInfo, for easier data retrieval
		* @author Daniel Konings
		*/
		class JSWrapper
		{
		public:
			enum Types
			{
				kVoid,
				kNumber,
				kBoolean,
				kString,
				kObject,
				kArray,
				kFunction,
				kNull,
				kUndefined,
				kUnknown
			};

			typedef v8::Local<v8::Object> Object;

		public:

			/**
			* @brief Construct with a given argument scope
			* @param[in] args (const v8::FunctionCallbackInfo<v8::Value>&) The arguments
			* @remarks Enters the isolate of the current JavaScript state
			*/
			JSWrapper(const v8::FunctionCallbackInfo<v8::Value>& args);

			/**
			* @brief Gets a value off of the argument stack
			* @param[in] arg (int) The argument to retrieve the value of
			* @param[in] def (const T&) The default value if no value was found
			*/
			template<typename T>
			T GetValue(int arg, const T& def);

			/**
			* @brief Gets a C++ pointer from a value with an external __ptr field
            * @param[in] val (const v8::Local<v8::Value>&) The value to retrieve the pointer from
			* @return (T*) The returned pointer, nullptr if none was found
			*/
			template<typename T>
            T* GetPointer(const v8::Local<v8::Value>& val);

			/**
			* @brief Gets a C++ pointer from a value with an external __ptr field
			* @param[in] arg (int) The argument to retrieve the pointer from
			* @return (T*) The returned pointer, nullptr if none was found
			*/
			template<typename T>
			T* GetPointer(int arg);

			/**
			* @brief Casts a value to a JavaScript handle
			* @param[in] val (const T&) The value to cast
            * @return (v8::Local<v8::Value>) The casted value
			*/
			template<typename T>
            static v8::Local<v8::Value> CastValue(const T& val);

			/**
			* @brief Returns a value to JavaScript
			* @param[in] val (const T&) The value to return
			*/
			template<typename T>
			void ReturnValue(const T& val);

			/**
			* @brief Sets an object value
            * @param[in] obj (const v8::Local<v8::Object>&) The object to assign the value to
			* @param[in] field (const snuffbox::engine::String&) The field to set
			* @param[in] val (const T&) The value to set
			*/
			template<typename T>
            static void SetObjectValue(const v8::Local<v8::Object>& obj, const engine::String& field, const T& val);

			/**
			* @brief Sets a function template value
			* @param[in] obj (const v8::Local<v8::FunctionTemplate>&) The function template to assign the value to
			* @param[in] field (const snuffbox::engine::String&) The field to set
			* @param[in] val (const T&) The value to set
			*/
			template<typename T>
			static void SetFunctionTemplateValue(const v8::Local<v8::FunctionTemplate>& obj, const engine::String& field, const T& val);

			/**
			* @brief Returns the type of a local value
			* @param[in] value (const v8::Local<v8::Value>&) The value to check the type of
			* @return (snuffbox::engine::JSWrapper::Types) The type of the value
			*/
			static Types TypeOf(const v8::Local<v8::Value>& value);

			/**
			* @brief Converts a JavaScript type to a string value
			* @param[in] type (snuffbox::engine::JSWrapper::Types) The type to convert
			* @return (snuffbox::engine::String) The converted string value
			*/
			static engine::String TypeToString(Types type);

			/**
			* @brief Creates a string from an UTF-8 value
			* @param[in] utf8 (const snuffbox::engine::String&) The string value to convert
			* @return (v8::Local<v8::String>) The converted V8 string
			*/
			static v8::Local<v8::String> CreateString(const engine::String& utf8);

			/**
			* @brief Creates a JavaScript object handle and returns it
			* @return (v8::Local<v8::Object>) The created object handle
			*/
			static v8::Local<v8::Object> CreateObject();

			/**
			* @brief Sets the private __ptr field of an object to a C++ pointer
			* @param[in] obj (const v8::Local<v8::Object>&) The object to assign the pointer to
			* @param[in] ptr (void*) The pointer to assign
			*/
			static void SetPointer(const v8::Local<v8::Object>& obj, void* ptr);

			/**
			* @brief Instantiates a native JavaScript object where T = JSObject
			* @param[in] args (Args&&...) The arguments to pass into the constructor
			* @return (v8::Local<v8::Object>) The instantiated JavaScript object
			*/
			template <typename T, typename ... Args>
			static v8::Local<v8::Object> New(Args&&...);

			/**
			* @brief Registers a global value
			* @param[in] name (const char*) The name to register the value under
			* @param[in] val (const v8::Local<v8::Value>&) The value to register
			* @param[in] in_namespace (bool) Should the global be registed in the 'snuff' namespace?
			*/
			static void RegisterGlobal(const char* name, const v8::Local<v8::Value>& val, bool in_namespace = false);

			/**
			* @brief Constructs an argument error and logs it
			* @param[in] expected (snuffbox::engine::JSWrapper::Types) The expected type
			* @param[in] got (snuffbox::engine::JSWrapper::Types) The type that was actually received
			* @param[in] arg (int) The current argument
			*/
			void Error(Types expected, Types got, int arg);

			/**
			* @brief Checks the argument scope if the format is as it should be
			* @param[in] format (const char*) The format to check
			* @param[in] force (bool) Should this check be enforced, even in Release mode? Default = true
			* @remarks e.g. B = Boolean, N = Number, concat the values like so; 'BN' in order of arguments
			* @return (bool) Was the format check completed succesfully?
			*/
			bool Check(const char* format, bool force = true);

			/**
			* @brief Disables error checking in snuffbox::engine::JSWrapper::Check
			* @param[in] value (bool) On/off value for error checking
			*/
			void set_error_checks(bool value);

			/**
			* @brief Default destructor
			* @remarks Exits the isolate of the current JavaScript state
			*/
			~JSWrapper();

		private:
			const v8::FunctionCallbackInfo<v8::Value>& args_; //!< The cached arguments
			bool error_checks_; //!< A boolean for multiple type checking
			JSStateWrapper::IsolateLock lock_; //!< The isolate lock for the wrapper
		};

		//-------------------------------------------------------------------------------------------
		template <>
		inline bool JSWrapper::GetValue<bool>(int arg, const bool& def)
		{
			if (args_[arg]->IsNull() || args_[arg]->IsUndefined())
			{
				return def;
			}

            if (args_[arg]->IsBoolean() == true)
            {
                v8::MaybeLocal<v8::Boolean> b = args_[arg]->ToBoolean(JSStateWrapper::Instance()->Context());
                return b.ToLocalChecked()->Value();
            }

            return def;
		}

        //-------------------------------------------------------------------------------------------
        template <>
        inline double JSWrapper::GetValue<double>(int arg, const double& def)
        {
            if (args_[arg]->IsNull() || args_[arg]->IsUndefined())
            {
                return def;
            }

            if (args_[arg]->IsNumber() == true)
            {
                v8::MaybeLocal<v8::Number> n = args_[arg]->ToNumber(JSStateWrapper::Instance()->Context());
                return static_cast<double>(n.ToLocalChecked()->Value());
            }
            return def;
        }

		//-------------------------------------------------------------------------------------------
		template <>
        inline int JSWrapper::GetValue<int>(int arg, const int& def)
		{
            return static_cast<int>(GetValue<double>(arg, def));
		}

		//-------------------------------------------------------------------------------------------
		template <>
		inline unsigned int JSWrapper::GetValue<unsigned int>(int arg, const unsigned int& def)
		{
			return static_cast<unsigned int>(GetValue<double>(arg, def));
		}

		//-------------------------------------------------------------------------------------------
		template <>
		inline float JSWrapper::GetValue<float>(int arg, const float& def)
        {
            return static_cast<float>(GetValue<double>(arg, def));
		}

		//-------------------------------------------------------------------------------------------
		template <>
		inline String JSWrapper::GetValue<String>(int arg, const String& def)
		{
			if (args_[arg]->IsNull() || args_[arg]->IsUndefined())
			{
				return def;
			}

            if (args_[arg]->IsString() == true)
            {
                v8::MaybeLocal<v8::String> str = args_[arg]->ToString(JSStateWrapper::Instance()->Context());
                return *v8::String::Utf8Value(str.ToLocalChecked());
            }

            return def;
		}

		//-------------------------------------------------------------------------------------------
		template <>
        inline v8::Local<v8::Value> JSWrapper::GetValue<v8::Local<v8::Value>>(int arg, const v8::Local<v8::Value>& def)
		{
			if (args_[arg]->IsNull() || args_[arg]->IsUndefined())
			{
				return def;
			}

            return args_[arg];
		}

		//-------------------------------------------------------------------------------------------
		template <typename T>
        inline T* JSWrapper::GetPointer(const v8::Local<v8::Value>& val)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Local<v8::Context> ctx = wrapper->Context();

			v8::Local<v8::Object> obj;
			bool maybe = val->ToObject(ctx).ToLocal(&obj);

			if (obj.IsEmpty() || obj->IsUndefined())
			{
				return nullptr;
			}

			v8::Local<v8::Value> ext = obj->GetPrivate(ctx, 
				v8::Private::ForApi(wrapper->isolate(), CreateString("__ptr"))).ToLocalChecked();

			if (ext.IsEmpty() && ext->IsExternal())
			{
				return nullptr;
			}
			else
			{
				return static_cast<T*>(ext.As<v8::External>()->Value());
			}
		}

		//-------------------------------------------------------------------------------------------
		template <typename T>
		inline T* JSWrapper::GetPointer(int arg)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Local<v8::Context> ctx = wrapper->Context();

			v8::Local<v8::Object> obj;
			args_[arg]->ToObject(ctx).ToLocal(&obj);

			if (obj.IsEmpty() || obj->IsUndefined())
			{
				return nullptr;
			}

			v8::Local<v8::Value> ext = obj->GetPrivate(ctx, 
				v8::Private::ForApi(wrapper->isolate(), CreateString("__ptr"))).ToLocalChecked();

			if (ext.IsEmpty() && ext->IsExternal())
			{
				return nullptr;
			}
			else
			{
				return static_cast<T*>(ext.As<v8::External>()->Value());
			}
		}

		//-------------------------------------------------------------------------------------------
		template <typename T>
		inline v8::Local<v8::Value> JSWrapper::CastValue(const T& val)
		{
			return val;
		}

		//-------------------------------------------------------------------------------------------
		template <>
        inline v8::Local<v8::Value> JSWrapper::CastValue<double>(const double& val)
		{
			v8::Isolate* isolate = JSStateWrapper::Instance()->isolate();
			return v8::Number::New(isolate, val);
		}

		//-------------------------------------------------------------------------------------------
		template <>
        inline v8::Local<v8::Value> JSWrapper::CastValue<int>(const int& val)
		{
			return CastValue<double>(static_cast<double>(val));
		}

		//-------------------------------------------------------------------------------------------
		template <>
		inline v8::Local<v8::Value> JSWrapper::CastValue<unsigned int>(const unsigned int& val)
		{
			return CastValue<double>(static_cast<double>(val));
		}

		//-------------------------------------------------------------------------------------------
		template <>
        inline v8::Local<v8::Value> JSWrapper::CastValue<float>(const float& val)
		{
			return CastValue<double>(static_cast<double>(val));
		}

		//-------------------------------------------------------------------------------------------
		template <>
        inline v8::Local<v8::Value> JSWrapper::CastValue<bool>(const bool& val)
		{
			v8::Isolate* isolate = JSStateWrapper::Instance()->isolate();
			return v8::Boolean::New(isolate, val);
		}

		//-------------------------------------------------------------------------------------------
		template <>
        inline v8::Local<v8::Value> JSWrapper::CastValue<engine::String>(const engine::String& val)
		{
			v8::Isolate* isolate = JSStateWrapper::Instance()->isolate();
            v8::MaybeLocal<v8::String> str = CreateString(val);
            return str.ToLocalChecked();
		}

		//-------------------------------------------------------------------------------------------
		template <typename T>
		inline void JSWrapper::ReturnValue(const T& val)
		{
			args_.GetReturnValue().Set<v8::Value>(CastValue<T>(val));
		}

		//-------------------------------------------------------------------------------------------
		template <typename T>
        inline void JSWrapper::SetObjectValue(const v8::Local<v8::Object>& obj, const engine::String& field, const T& val)
		{
            JSStateWrapper* wrapper = JSStateWrapper::Instance();
            v8::Isolate* isolate = wrapper->isolate();

            obj->Set(wrapper->Context(), CreateString(field), CastValue<T>(val));
		}

		//-------------------------------------------------------------------------------------------
		template <typename T>
		inline void JSWrapper::SetFunctionTemplateValue(const v8::Local<v8::FunctionTemplate>& obj, const engine::String& field, const T& val)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			obj->Set(isolate, field.c_str(), CastValue<T>(val));
		}

		//-------------------------------------------------------------------------------------------
		template <typename T, typename ... Args>
		inline v8::Local<v8::Object> JSWrapper::New(Args&&... args)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			T* ptr = Memory::default_allocator().Construct<T>(args...);

			v8::Local<v8::Object> obj = CreateObject();
			ptr->object().Reset(isolate, obj);
			ptr->object().SetWeak(ptr, JSStateWrapper::Destroy<T>, v8::WeakCallbackType::kParameter);
			ptr->object().MarkIndependent();
			obj->SetPrivate(wrapper->Context(),
				v8::Private::ForApi(isolate, v8::String::NewFromUtf8(isolate, "__ptr", v8::NewStringType::kNormal).ToLocalChecked()),
				v8::External::New(isolate, static_cast<void*>(ptr)));

			int64_t size = static_cast<int64_t>(sizeof(T));
			isolate->AdjustAmountOfExternalAllocatedMemory(size);

			return obj;
		}
	}
}
