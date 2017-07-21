#pragma once

#include "../core/eastl.h"
#include <time.h>

namespace snuffbox
{
	namespace engine
	{
		class ContentManager;

		/**
		* @class snuffbox::engine::FileWatch
		* @brief A file watcher that watches for files that have been modified and reloads them through the content manager
		* @author Daniel Konings
		*/
		class FileWatch
		{
			friend class ContentManager;

		protected:

			/**
			* @brief Construct by providing the content manager this file watch is owned by
			* @param[in] cm (snuffbox::engine::ContentManager*) The current content manager that owns this file watch
			*/
			FileWatch(ContentManager* cm);

			/**
			* @brief Retrieves the file time of a file by path
			* @param[in] path (const snuffbox::engine::String&) The path to the file
			* @return (tm) The time the file was last modified at
			*/
			tm GetFileTime(const String& path);

			/**
			* @brief Compares two times and returns if they are different
			* @param[in] last (tm) The last time
			* @param[in] now (tm) The current time
			* @return (bool) Are the times different?
			*/
			static bool Compare(tm last, tm now);

			/**
			* @brief Adds a file to be watched for changes
			* @param[in] path (const snuffbox::engine::String&) The path to the file
			*/
			void Add(const String& path);

			/**
			* @brief Removes a file being watched, so it will not be reloaded anymore
			* @param[in] path (const snuffbox::engine::String&) The path to the file
			*/
			void Remove(const String& path);

			/**
			* @brief Updates the file watch and reloads any files where necessary
			*/
			void Update();

		private:

			typedef Map<String, tm> FileTimeMap;

			FileTimeMap file_times_; //!< The last modified times of all loaded files
			ContentManager* content_manager_; //!< The current content manager that owns this file watch

			unsigned int frame_count_; //!< The current frame count
		};
	}
}