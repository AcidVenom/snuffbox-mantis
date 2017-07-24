#pragma once

#include "../services/cvar_service.h"
#include "../memory/memory.h"

#include "../js/js_defines.h"

namespace snuffbox
{
	namespace engine
	{
		class SnuffboxApp;
		class Allocator;

		/**
		* @class snuffbox::engine::CVar : [JSObject] public snuffbox::engine::CVarService
		* @brief A CVar service to store and retrieve CVars set via the command line or console
		* @author Daniel Konings
		*/
		class CVar : JS_OBJECT_MULTI public CVarService
		{

			friend class SnuffboxApp;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor
			*/
			CVar();

			/**
			* @brief Shutsdown the CVar system
			*/
			void Shutdown();

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
			void SetBoolean(const String& name, bool value) override;

			/**
			* @see snuffbox::engine::CVarService::SetNumber
			*/
			void SetNumber(const String& name, float value) override;

			/**
			* @see snuffbox::engine::CVarService::GetString
			*/
			CVarString* GetString(const String& name) override;

			/**
			* @see snuffbox::engine::CVarService::GetBoolean
			*/
			CVarBoolean* GetBoolean(const String& name) override;

			/**
			* @see snuffbox::engine::CVarService::GetNumber
			*/
			CVarNumber* GetNumber(const String& name) override;

			/**
			* @brief Finds a specific CVarValue by name
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to find
			* @return (T*) The found CVar, or nullptr if it doesn't exist
			*/
			template <typename T>
			T* Find(const String& name);

			/**
			* @brief Sets a specific CVarValue by name and its typed value
			* @param[in] name (const snuffbox::engine::String&) The name of the CVar to set
			* @param[in] value (snuffbox::engine::CVarBase::value_type<T>::type) The value to set
			*/
			template <typename T>
            void DoSet(const String& name, typename CVarBase::value_type<T>::type value);

		private:

			typedef Map<String, SharedPtr<CVarBase>> CVarMap;

			CVarMap cvars_[CVarBase::CVarTypes::kCount]; //!< All the currently stored CVars

		public:

			JS_NAME_SINGLE(CVar);
			JS_FUNCTION_DECL(set);
			JS_FUNCTION_DECL(get);
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		T* CVar::Find(const String& name)
		{
			CVarMap::iterator it = cvars_[T::TYPE_ID].find(name);

			if (it != cvars_[T::TYPE_ID].end())
			{
				return static_cast<T*>(it->second.get());
			}

			return nullptr;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
        void CVar::DoSet(const String& name, typename CVarBase::value_type<T>::type value)
		{
			CVarMap::iterator it = cvars_[T::TYPE_ID].find(name);

			if (it != cvars_[T::TYPE_ID].end())
			{
				static_cast<T*>(it->second.get())->set_value(value);
				return;
			}

			cvars_[T::TYPE_ID].emplace(name, Memory::ConstructShared<T>(value));
		}
	}
}
