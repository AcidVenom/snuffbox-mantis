#pragma once

#include <type_traits>
#include "../memory/memory.h"
#include "../js/js_defines.h"

namespace snuffbox
{
	namespace engine
	{
		class File;
		class ContentManager;
		class ContentBase;

		/**
		* @class snuffbox::engine::ContentPtr
		* @brief Used to keep track of content with a ref count and if it's still valid
		* @author Daniel Konings
		*/
		template <typename T>
		class ContentPtr JS_OBJECT
		{

			friend class ContentManager;
			friend class ContentService;

		public:

			/**
			* @brief Default constructor
			*/
			ContentPtr();

		protected:

			/**
			* @brief Construct by providing the actual raw pointer
			* @param[in] ptr (T*) The pointer to the actual content
			*/
			ContentPtr(T* ptr);

			/**
			* @brief Construct a content pointer from one type to the other
			* @param[in] other (const snuffbox::engine::ContentPtr<Y>&) The other pointer of a different type
			*/
			template <typename Y>
			ContentPtr(const ContentPtr<Y>& other);

		public:

			/**
			* @brief Retrieves the raw content pointer
			* @remarks Returns nullptr if the content is no longer loaded, resetting the shared pointer
			* @return (T*) The raw pointer
			*/
			T* Get();

			/**
			* @brief Does a snuffbox::engine::ContentPtr<T>::Get and retrieves the raw pointer
			* @see snuffbox::engine::ContentPtr<T>::Get
			* @return (T*) The raw pointer, or nullptr if this content was not valid
			*/
			T* operator->();

			/**
			* @return (const snuffbox::engine::SharedPtr<snuffbox::engine::ContentBase>&) The underlying shared pointer
			*/
			const SharedPtr<ContentBase>& ptr() const;

		private:

			SharedPtr<ContentBase> ptr_; //!< The shared pointer that keeps the reference count for this contained pointer
		};

		/**
		* @class snuffbox::engine::ContentBase
		* @brief The base class for every type of content to use
		* @author Daniel Konings
		*/
		class ContentBase
		{

			friend class ContentManager;

		public:

			/**
			* @brief Default constructor
			*/
			ContentBase();

			/**
			* @brief The types of content available
			*/
			enum Types : int
			{
				kScript, //!< The JavaScript file type
				kCount //!< The total number of types
			};

			/**
			* @brief Loads the actual data from a provided file
			* @param[in] file (snuffbox::engine::File*) The file to load the data of this content from
			* @return (bool) Was the load a success?
			*/
			virtual bool Load(File* file) = 0;

			/**
			* @brief Reloads the actual data from a provided file
			* @param[in] file (snuffbox::engine::File*) The file to reload the data of this content from
			* @return (bool) Was the reload a success?
			*/
			virtual bool Reload(File* file) = 0;

		protected:

			/**
			* @brief Sets if this content is valid
			* @param[in] valid (bool) The boolean value
			*/
			void set_is_valid(bool valid);

		public:

			/**
			* @return (bool) Is this content still valid?
			*/
			bool is_valid() const;

			/**
			* @brief Default destructor
			*/
			virtual ~ContentBase();

		private:

			bool is_valid_; //!< Is this content still valid?
		};

		/**
		* @class snuffbox::engine::Content<int> : public snuffbox::engine::ContentBase
		* @brief The typed base class for every piece of content to use
		* @author Daniel Konings
		*/
		template <int T>
		class Content : public ContentBase
		{

			friend class ContentService;

		public:

			/**
			* @brief Default destructor
			*/
			virtual ~Content();

		protected:

			static const int CONTENT_ID = T; //!< The type ID of this piece of content
		};

		/**
		* @struct snuffbox::engine::is_content<T>
		* @brief Used to check if a class is actually a piece of content
		* @author Daniel Konings
		*/
		template <typename T>
		struct is_content
		{
			static const bool value = std::is_base_of<ContentBase, T>::value;
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline ContentPtr<T>::ContentPtr() :
			ptr_(nullptr)
		{

		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline ContentPtr<T>::ContentPtr(T* ptr) :
			ptr_(Memory::MakeShared<ContentBase>(static_cast<ContentBase*>(ptr)))
		{

		}

		//-----------------------------------------------------------------------------------------------
		template <typename T> template <typename Y>
		inline ContentPtr<T>::ContentPtr(const ContentPtr<Y>& other)
		{
			ptr_ = other.ptr();
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		const SharedPtr<ContentBase>& ContentPtr<T>::ptr() const
		{
			return ptr_;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline T* ContentPtr<T>::Get()
		{
			if (ptr_ == nullptr || ptr_->is_valid() == false)
			{
				ptr_.reset();
				return nullptr;
			}

			return static_cast<T*>(ptr_.get());
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline T* ContentPtr<T>::operator->()
		{
			return Get();
		}

		//-----------------------------------------------------------------------------------------------
		template <int T>
		Content<T>::~Content()
		{

		}
	}
}
