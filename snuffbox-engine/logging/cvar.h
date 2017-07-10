#pragma once

#include "../services/cvar_service.h"

namespace snuffbox
{
	namespace engine
	{
		class Application;
		class Allocator;

		/**
		* @class snuffbox::engine::CVar
		* @brief A CVar service to store and retrieve CVars set via the command line or console
		* @author Daniël Konings
		*/
		class CVar : public CVarService
		{
			friend class Application;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor
			*/
			CVar();

			/**
			* @brief Parses the command line and adds the corresponding values to the list of CVar values
			* @param[in] argc (const int&) The number of command line arguments
			* @param[in] argv (char**) The actual command line arguments
			*/
			void ParseCommandLine(const int& argc, char** argv);

		public:

			/**
			* @see snuffbox::engine::CVarService::LogAll
			*/
			void LogAll() override;

			/**
			* @see snuffbox::engine::CVarService::SetString
			*/
			void SetString(const String& name, const String& value) override;

			/**
			* @see snuffbox::engine::CVarService::SetBoolean
			*/
			void SetBoolean(const String& name, const bool& value) override;

			/**
			* @see snuffbox::engine::CVarService::SetNumber
			*/
			void SetNumber(const String& name, const float& value) override;

			/**
			* @see snuffbox::engine::CVarService::GetString
			*/
			CVarString* GetString(const String& name) const override;

			/**
			* @see snuffbox::engine::CVarService::GetBoolean
			*/
			CVarBoolean* GetBoolean(const String& name) const override;

			/**
			* @see snuffbox::engine::CVarService::GetNumber
			*/
			CVarNumber* GetNumber(const String& name) const override;

		private:

			Map<String, CVarBase> cvars_[CVarBase::CVarTypes::kCount]; //!< All the currently stored CVars
		};
	}
}