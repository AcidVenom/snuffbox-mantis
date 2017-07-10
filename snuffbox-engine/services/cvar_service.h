#pragma once

#include "service.h"
#include "services.h"

#include "../logging/cvar_value.h"

namespace snuffbox
{
	namespace engine
	{
		/**
		* @class snuffbox::engine::CVarService : public snuffbox::engine::Service<snuffbox::engine::ServiceIDs::kCVarService>
		* @brief The CVar service interface for every CVar service to use
		* @author Daniël Konings
		*/
		class CVarService : public Service<ServiceIDs::kCVarService>
		{

			friend class Services;

		protected:

			/**
			* @brief Default constructor, creates a null service
			*/
			CVarService();

		public:

			/**
			* @brief Logs all CVars
			*/
			virtual void LogAll();

		protected:

			/**
			* @brief Sets a string typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name to save the CVar under
			* @param[in] value (const snuffbox::engine::String&) The string value to set
			*/
			virtual void SetString(const String& name, const String& value);

			/**
			* @brief Sets a boolean typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name to save the CVar under
			* @param[in] value (const bool&) The boolean value to set
			*/
			virtual void SetBoolean(const String& name, const bool& value);

			/**
			* @brief Sets a number typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name to save the CVar under
			* @param[in] value (const float&) The number value to set
			*/
			virtual void SetNumber(const String& name, const float& value);

			/**
			* @brief Retrieves a string typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to retrieve
			* @return (snuffbox::engine::CVarString*) The found CVar, or nullptr if it doesn't exist
			*/
			virtual CVarString* GetString(const String& name) const;

			/**
			* @brief Retrieves a boolean typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to retrieve
			* @return (snuffbox::engine::CVarBoolean*) The found CVar, or nullptr if it doesn't exist
			*/
			virtual CVarBoolean* GetBoolean(const String& name) const;

			/**
			* @brief Retrieves a number typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to retrieve
			* @return (snuffbox::engine::CVarNumber*) The found CVar, or nullptr if it doesn't exist
			*/
			virtual CVarNumber* GetNumber(const String& name) const;
		};
	}
}