#pragma once

#include "service.h"
#include "services.h"

#include "log_service.h"

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

			/**
			* @brief Checks if a string value is a number and fills in the buffer with the number if it is
			* @param[in] value (char*) The string value to check
			* @param[out] buffer (float*) The buffer to assign the numerical float value to
			* @return (bool) Was the string a number?
			*/
			static bool ParseNumber(char* value, float* buffer);

		public:

			/**
			* @brief Delete copy constructor
			*/
			CVarService(const CVarService& other) = delete;

			/**
			* @brief Delete assignment operator
			*/
			CVarService operator=(const CVarService& other) = delete;

			/**
			* @brief Parses the command line and adds the corresponding values to the list of CVar values
			* @param[in] argc (const int&) The number of command line arguments
			* @param[in] argv (char**) The actual command line arguments
			*/
			void ParseCommandLine(const int& argc, char** argv);

			/**
			* @brief Logs all CVars
			*/
			virtual void LogAll();

			/**
			* @brief Sets a CVar value of type T with a specified name and value
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to set
			* @param[in] value (const snuffbox::engineCVarBase::value_type<T>::type&) The value to set
			* @see snuffbox::engine::CVarValue<T>::VALUE_TYPE
			*/
			template <typename T>
            void Set(const String& name, const typename CVarBase::value_type<T>::type& value);

			/**
			* @brief Retrieves a CVar value by name
			* @param[in] name (const snuffbox::engine::String&) The CVar to retrieve
			* @return (T*) The found CVar, or nullptr if it doesn't exist
			*/
			template <typename T>
			T* Get(const String& name);

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
			virtual CVarString* GetString(const String& name);

			/**
			* @brief Retrieves a boolean typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to retrieve
			* @return (snuffbox::engine::CVarBoolean*) The found CVar, or nullptr if it doesn't exist
			*/
			virtual CVarBoolean* GetBoolean(const String& name);

			/**
			* @brief Retrieves a number typed CVar value
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to retrieve
			* @return (snuffbox::engine::CVarNumber*) The found CVar, or nullptr if it doesn't exist
			*/
			virtual CVarNumber* GetNumber(const String& name);
		};

		//-----------------------------------------------------------------------------------------------
		template <>
        inline void CVarService::Set<CVarString>(const String& name, const typename CVarBase::value_type<CVarString>::type& value)
		{
			if (name.find(' ') != -1)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kWarning, "CVar '{0}' could not be set, CVars cannot contain spaces", name.c_str());
				return;
			}
			SetString(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		template <>
        inline void CVarService::Set<CVarBoolean>(const String& name, const typename CVarBase::value_type<CVarBoolean>::type& value)
		{
			if (name.find(' ') != -1)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kWarning, "CVar '{0}' could not be set, CVars cannot contain spaces", name.c_str());
				return;
			}
			SetBoolean(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		template <>
        inline void CVarService::Set<CVarNumber>(const String& name, const typename CVarBase::value_type<CVarNumber>::type& value)
		{
			if (name.find(' ') != -1)
			{
				Services::Get<LogService>().Log(console::LogSeverity::kWarning, "CVar '{0}' could not be set, CVars cannot contain spaces", name.c_str());
				return;
			}
			SetNumber(name, value);
		}

		//-----------------------------------------------------------------------------------------------
		template <>
		inline CVarString* CVarService::Get<CVarString>(const String& name)
		{
			return GetString(name);
		}

		//-----------------------------------------------------------------------------------------------
		template <>
		inline CVarBoolean* CVarService::Get<CVarBoolean>(const String& name)
		{
			return GetBoolean(name);
		}

		//-----------------------------------------------------------------------------------------------
		template <>
		inline CVarNumber* CVarService::Get<CVarNumber>(const String& name)
		{
			return GetNumber(name);
		}
	}
}
