#pragma once

#include "service.h"
#include "services.h"
#include "../core/eastl.h"

#include <snuffbox-console/logging/logging.h>

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::LogService : public snuffbox::engine::Service<snuffbox::engine::ServiceIDs::kLogService>
		* @brief The logging service interface for all log services to use
		* @author Daniël Konings
		*/
		class LogService : public Service<ServiceIDs::kLogService>
		{

		public:

			/**
			* @brief Default constructor, creates a null service
			*/
			LogService();

			/**
			* @brief Prints a debug message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Debug(const String& message);

			/**
			* @brief Prints an info message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Info(const String& message);

			/**
			* @brief Prints a success message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Success(const String& message);

			/**
			* @brief Prints a warning message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Warning(const String& message);

			/**
			* @brief Prints an error message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Error(const String& message);

			/**
			* @brief Prints a fatal message
			* @param[in] message (const snuffbox::engine::String&) The message to log
			*/
			virtual void Fatal(const String& message);

			/**
			* @brief Prints an RGB message with a specified colour
			* @param[in] message (const snuffbox::engine::String&) The message to log
			* @param[in] colour (const snuffbox::console::LogColour&) The colour to log with
			*/
			virtual void RGB(const String& message, const console::LogColour& colour);

			/**
			* @brief Cross-platform assert with a message
			* @param[in] expr (const bool&) The expression to evaluate
			* @param[in] message (const snuffbox::engine::String&) The message to log with fatal severity if the evaluation was false
			*/
			virtual void Assert(const bool& expr, const String& message);
		};
	}
}