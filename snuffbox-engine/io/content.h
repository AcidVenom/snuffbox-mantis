#pragma once

namespace snuffbox
{
	namespace engine
	{
		class File;

		template <int T>
		class Content;
		class ContentManager;
		class ContentService;

		/**
		* @struct snuffbox::engine::ContentBase
		* @brief The base class for every type of content to use
		* @author Daniël Konings
		*/
		struct ContentBase
		{
			template <int T>
			friend class Content;
			friend class ContentManager;
			friend class ContentService;

		protected:

			/**
			* @brief The types of content available
			*/
			enum Types : int
			{
				kJavaScript, //!< The JavaScript file type
				kCount //!< The total number of types
			};
		};

		/**
		* @class snuffbox::engine::Content<int> : public snuffbox::engine::ContentBase
		* @brief The typed base class for every piece of content to use
		* @author Daniël Konings
		*/
		template <int T>
		class Content : public ContentBase
		{

			friend class ContentService;

		public:

			/**
			* @brief Loads the actual data from a provided file
			* @param[in] file (const snuffbox::engine::File*) The file to load the data of this content from
			*/
			virtual void Load(const File* file) = 0;

			/**
			* @brief Reloads the actual data from a provided file
			* @param[in] file (const snuffbox::engine::File*) The file to reload the data of this content from
			*/
			virtual void Reload(const File* file) = 0;

		protected:

			static const int CONTENT_ID = T; //!< The type ID of this piece of content
		};

		/**
		* @struct snuffbox::engine::is_content<T>
		* @brief Used to check if a class is actually a piece of content
		* @author Daniël Konings
		*/
		template <typename T>
		struct is_content
		{
			static const bool value = std::is_base_of<ContentBase, T>::value;
		};
	}
}