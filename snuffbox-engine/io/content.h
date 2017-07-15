#pragma once

namespace snuffbox
{
	namespace engine
	{
		class File;

		/**
		* @class snuffbox::engine::ContentBase
		* @brief The base class for every type of content to use
		* @author Daniël Konings
		*/
		class ContentBase
		{

		public:

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