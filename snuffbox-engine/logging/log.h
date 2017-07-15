#pragma once

#include "../services/log_service.h"
#include <snuffbox-logging/logging_stream.h>

#include "log_client.h"

#include "../js/js_defines.h" 

namespace snuffbox
{
	namespace engine
	{
		class SnuffboxApp;
		class Allocator;
		class CVar;

		/**
		* @class snuffbox::engine::Log : public snuffbox::engine::LogService
		* @brief The logging system to log all types of different messages and throw assertions with
		* @author Daniël Konings
		*/
		class Log : JS_OBJECT_MULTI public LogService
		{

			friend class SnuffboxApp;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor
			*/
			Log();

			/**
			* @brief Initialises the log system
			* @param[in] cvar (snuffbox::engine::CVar*) The CVar system
			*/
			void Initialise(CVar* cvar);

			/**
			* @brief Shuts down the logging system
			*/
			void Shutdown();

			/**
			* @see snuffbox::engine::LogService::Debug
			*/
			void Debug(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Info
			*/
			void Info(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Success
			*/
			void Success(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Warning
			*/
			void Warning(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Error
			*/
			void Error(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::Fatal
			*/
			void Fatal(const String& message) override;

			/**
			* @see snuffbox::engine::LogService::RGB
			*/
			void RGB(const String& message, const console::LogColour& colour) override;

			/**
			* @see snuffbox::engine::LogService::DoAssert
			*/
			void DoAssert(const bool& expr, const String& message) override;

		private:

			bool enabled_; //!< Has the console been enabled?
			LogClient client_; //!< The logging client
			logging::LoggingStream stream_; //!< The logging stream

		public:

			JS_NAME_SINGLE(Log);

			JS_FUNCTION_DECL(debug);
			JS_FUNCTION_DECL(info);
			JS_FUNCTION_DECL(success);
			JS_FUNCTION_DECL(warning);
			JS_FUNCTION_DECL(error);
			JS_FUNCTION_DECL(fatal);
			JS_FUNCTION_DECL(rgb);
		};
	}
}