#pragma once

#include "js_wrapper.h"
#include "../services/log_service.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::JSCallback<Args...>
		* @brief Used to set a callback from JavaScript and call it from C++
		* @author Daniel Konings
		*/
		template <typename ...Args>
		class JSCallback
		{

		public:
				
			/**
			* @brief Default constructor
			*/
			JSCallback();

			/**
			* @brief Used to set weak callback data for this callback
			*/
			static void JSWeakCallback(const v8::WeakCallbackInfo<JSCallback<Args...>>& data);

			/**
			* @brief Sets the callback to a field in the global object
			* @param[in] cb (const snuffbox::engine::String&) The name of the field to assign to the callback
			* @return (bool) Was setting the callback a success? Fails if; undefined or not of a function type
			*/
			bool Set(const engine::String& cb);

			/**
			* @brief Sets the callback to a field of an object in the global object
			* @param[in] obj (const snuffbox::engine::String&) The name of the object to retrieve from the global object
			* @param[in] field (const snuffbox::engine::String&) The name of the field in the object to assign to the callback
			* @return (bool) Was setting the callback a success? Fails if; object/field is undefined or field is not of a function type
			*/
			bool Set(const engine::String& obj, const engine::String& field);

			/**
			* @brief Sets the callback from a provided function
			* @param[in] cb (const v8::Local<v8::Value>&) The callback to set
			*/
			void Set(const v8::Local<v8::Value>& cb);

			/**
			* @brief Pushes a value into the value array, to pass to the call of the callback
			* @param[in] arg (const T&) The argument to push
			*/
			template <typename T>
			void Push(const T& arg);

			/**
			* @brief The last function to break recursion of argument pushing
			* @return (int) 0, as there are no more arguments to push
			*/
			int PushArg();

			/**
			* @brief Pushes an argument when there is only one argument left
			* @param[in] first (const T&) The argument that's left to be pushed
			* @return (int) 1, still arguments on the stack
			*/
			template <typename T>
			int PushArg(const T& first);

			/**
			* @brief Pushes an argument when there are 2 or more arguments left
			* @param[in] first (const T&) The argument that's next to be pushed
			* @param[in] args (const Others&...) The rest of the arguments that are left to be pushed
			* @return (int) 1 + previous argument count, still arguments on the stack
			*/
			template <typename T, typename ... Others>
			int PushArg(const T& first, const Others&... args);

			/**
			* @brief Calls the function JavaScript sided
			* @param[in] args (const Args&...) The arguments to forward to JavaScript
			*/
			void Call(const Args&... args);

			/**
			* @brief Clears the weak data of the persistent callback handle
			*/
			void Clear();

			/**
			* @brief Default destructor, destroys the persistent object
			*/
			~JSCallback();

		private:
			v8::Persistent<v8::Function> callback_; //!< A persistent handle containing the callback if any
			engine::Vector<v8::Local<v8::Value>> values_; //!< The arguments to call with JavaScript sided
			bool valid_; //!< Is this callback valid?
		};

		//-------------------------------------------------------------------------------------------
		template<typename ... Args>
		inline JSCallback<Args...>::JSCallback() :
			valid_(false)
		{

		}

		//-------------------------------------------------------------------------------------------
		template<typename ... Args>
		inline bool JSCallback<Args...>::Set(const engine::String& cb)
		{
			LogService& log = Services::Get<LogService>();

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			JSStateWrapper::IsolateLock lock(isolate);

			v8::Local<v8::Object> global = wrapper->Global();
			v8::Local<v8::Value> value;
			global->Get(wrapper->Context(), JSWrapper::CreateString(cb)).ToLocal(&value);

			if (value.IsEmpty() == true || value->IsUndefined())
			{
				log.Log(console::LogSeverity::kError, "JavaScript callback: could not find value '{0}'", cb);
				return false;
			}

			if (value->IsFunction() == false)
			{
				log.Log(console::LogSeverity::kError, "JavaScript callback: callback with name '{0}' is not of a function type", cb);
				return false;
			}

			callback_.Reset(isolate, value.As<v8::Function>());
			callback_.SetWeak(static_cast<JSCallback<Args...>*>(this), JSWeakCallback, v8::WeakCallbackType::kParameter);
			valid_ = true;

			return true;
		}

		//-------------------------------------------------------------------------------------------
		template<typename ... Args>
		inline bool JSCallback<Args...>::Set(const engine::String& obj, const engine::String& field)
		{
			LogService& log = Services::Get<LogService>();

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			JSStateWrapper::IsolateLock lock(isolate);

			callback_.Reset();

			v8::Local<v8::Object> global = wrapper->Global();
			v8::Local<v8::Context> ctx = wrapper->Context();
			v8::Local<v8::Value> object;
			bool maybe = global->Get(ctx, JSWrapper::CreateString(obj)).ToLocal(&object);

			if (object.IsEmpty() == true || object->IsUndefined())
			{
				log.Log(console::LogSeverity::kError, "JavaScript callback: could not find object '{0}'", obj);
				return false;
			}

			if (object->IsObject() == false)
			{
				log.Log(console::LogSeverity::kError, "JavaScript callback: value with name '{0}' is not of an object type", obj);
				return false;
			}

			v8::Local<v8::Value> value = object->ToObject(ctx).ToLocalChecked()->Get(
				ctx, 
				JSWrapper::CreateString(field)).ToLocalChecked();

			if (value.IsEmpty() == true || value->IsUndefined())
			{
				log.Log(console::LogSeverity::kError, "JavaScript callback: could not find callback '{0}.{1}'", obj, field);
				return false;
			}

			if (value->IsFunction() == false)
			{
				log.Log(console::LogSeverity::kError, "JavaScript callback: callback '{0}.{1}' is not of a function type", obj, field);
				return false;
			}

			callback_.Reset(isolate, value.As<v8::Function>());
			callback_.SetWeak(static_cast<JSCallback<Args...>*>(this), JSWeakCallback, v8::WeakCallbackType::kParameter);
			valid_ = true;

			return true;
		}

		//-------------------------------------------------------------------------------------------
		template<typename ... Args>
		inline void JSCallback<Args...>::Set(const v8::Local<v8::Value>& cb)
		{
			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();
			JSStateWrapper::IsolateLock lock(isolate);

			v8::Local<v8::Value> value = cb;
			callback_.Reset(isolate, value.As<v8::Function>());
			callback_.SetWeak(static_cast<JSCallback<Args...>*>(this), JSWeakCallback, v8::WeakCallbackType::kParameter);
			valid_ = true;
		}

		//-------------------------------------------------------------------------------------------
		template<typename ... Args> template<typename T>
		inline void JSCallback<Args...>::Push(const T& arg)
		{
			values_.push_back(JSWrapper::CastValue<T>(arg));
		}

		//-------------------------------------------------------------------------------------------
		template <typename ... Args>
		inline int JSCallback<Args...>::PushArg()
		{
			return 0;
		}

		//-------------------------------------------------------------------------------------------
		template <typename ... Args> template <typename T>
		inline int JSCallback<Args...>::PushArg(const T& first)
		{
			Push(first);
			return 1;
		}

		//-------------------------------------------------------------------------------------------
		template <typename ... Args> template <typename T, typename ... Others>
		inline int JSCallback<Args...>::PushArg(const T& first, const Others&...args)
		{
			Push(first);
			return PushArg(args...) + 1;
		}

		//-------------------------------------------------------------------------------------------
		template <typename ...Args>
		inline void JSCallback<Args...>::Call(const Args& ...args)
		{
			if (valid_ == false)
			{
				return;
			}

			JSStateWrapper* wrapper = JSStateWrapper::Instance();
			v8::Isolate* isolate = wrapper->isolate();

			JSStateWrapper::IsolateLock lock(isolate);

			v8::Local<v8::Context> ctx = wrapper->Context();

			v8::Local<v8::Function> func = v8::Local<v8::Function>::New(isolate, callback_);
			v8::Local<v8::Value> fctx;
			bool maybe = func->Get(ctx, JSWrapper::CreateString("ctx")).ToLocal(&fctx);

			v8::TryCatch try_catch;

			values_.clear();
			int nargs = PushArg(args...);

			nargs != 0 ? func->Call(ctx,
				fctx->IsUndefined() == false && fctx->IsObject() == true ?
				fctx->ToObject(ctx).ToLocalChecked() : wrapper->Global(), nargs, &values_[0])

				:

				func->Call(ctx,
					fctx->IsUndefined() == false && fctx->IsObject() == true ?
					fctx->ToObject(ctx).ToLocalChecked() : wrapper->Global(), 0, 0);

			engine::String exception;
			bool failed = wrapper->GetException(&try_catch, &exception);

			if (failed == true)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kError, exception);
			}
		}

		//-------------------------------------------------------------------------------------------
		template<typename ... Args>
		inline void JSCallback<Args...>::Clear()
		{
			if (valid_ == false)
			{
				return;
			}

			if (callback_.IsEmpty() == false)
			{
				callback_.ClearWeak();
				callback_.Reset();
			}
		}

		//-------------------------------------------------------------------------------------------
		template<typename ... Args>
		inline JSCallback<Args...>::~JSCallback()
		{
			Clear();
			valid_ = false;
		}

		//-------------------------------------------------------------------------------------------
		template<typename ... Args>
		inline void JSCallback<Args...>::JSWeakCallback(const v8::WeakCallbackInfo<JSCallback<Args...>>& data)
		{
			JSCallback* ptr = data.GetParameter();
			ptr->Clear();
		}
	}
}
