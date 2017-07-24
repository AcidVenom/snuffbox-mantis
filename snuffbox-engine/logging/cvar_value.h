#pragma once

#include "../core/eastl.h"

namespace snuffbox
{
	namespace engine
	{
		class CVarString;
		class CVarBoolean;
		class CVarNumber;
		class CVarService;
		class CVar;
		class Allocator;

		/**
		* @class snuffbox::engine::CVarBase
		* @brief The base class of every CVar value
		* @author Daniel Konings
		*/
		class CVarBase
		{
			friend class CVarString;
			friend class CVarBoolean;
			friend class CVarNumber;
			friend class CVarService;
			friend class CVar;

		protected:

			/**
			* @brief The different CVar value types
			*/
			enum CVarTypes : int
			{
				kString, //!< A string value
				kBoolean, //!< A boolean value
				kNumber, //!< A number value
				kCount //!< The number of types
			};

			/**
			* @struct snuffbox::engine::CVarBase::value_type<T>
			* @brief Used to determine the value type of a CVar
			* @author Daniel Konings
			*/
			template <typename T>
			struct value_type
			{

			};
		};

        /**
        * @struct snuffbox::engine::CVarBase::value_type<CVarString>
        * @brief A CVarString's value type
        * @author Daniel Konings
        */
        template <>
        struct CVarBase::value_type<CVarString>
        {
            typedef String type;
        };

        /**
        * @struct snuffbox::engine::CVarBase::value_type<CVarBoolean>
        * @brief A CVarBoolean's value type
        * @author Daniel Konings
        */
        template <>
        struct CVarBase::value_type<CVarBoolean>
        {
            typedef bool type;
        };

        /**
        * @struct snuffbox::engine::CVarBase::value_type<CVarNumber>
        * @brief A CVarNumber's value type
        * @author Daniel Konings
        */
        template <>
        struct CVarBase::value_type<CVarNumber>
        {
            typedef float type;
        };

		/**
		* @class snuffbox::engine::CVarValue<T, Y> : public snuffbox::engine::CVarBase
		* @brief A CVarValue containing the actual typed value provided by T
		* @author Daniel Konings
		*/
		template <typename T, int Y>
		class CVarValue : public CVarBase
		{
			friend class CVarService;
			friend class CVar;

		protected:

			/**
			* @brief Default constructor, initialises the CVar
			* @param[in] init (const T&) The initial value
			*/
			CVarValue(const T& init);

		public:

			/**
			* @brief Sets the typed value of this CVar
			* @param[in] value (const T&) The value to set
			*/
			void set_value(const T& value);

			/**
			* @return (const T&) The actual typed value of this CVar
			*/
			const T& value() const;

		protected:

			const static int TYPE_ID = Y; //!< The type ID of this CVar

		private:

			T value_; //!< The actual typed value of this CVar
		};

		/**
		* @class snuffbox::engine::CVarString : public snuffbox::engine::CVarValue<snuffbox::engine::String, snuffbox::engine::CVarBase::CVarTypes::kNumber>
		* @brief A CVar to store a string value
		* @author Daniel Konings
		*/
		class CVarString : public CVarValue<String, CVarBase::CVarTypes::kString>
		{

			friend class CVar;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor, initialises the CVar
			* @param[in] init (const snuffbox::engine::String&) The initial value
			*/
			CVarString(const String& init);
		};

		/**
		* @class snuffbox::engine::CVarString : public snuffbox::engine::CVarValue<bool, snuffbox::engine::CVarBase::CVarTypes::kBoolean>
		* @brief A CVar to store a boolean value
		* @author Daniel Konings
		*/
		class CVarBoolean : public CVarValue<bool, CVarBase::CVarTypes::kBoolean>
		{

			friend class CVar;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor, initialises the CVar
			* @param[in] init (bool) The initial value
			*/
			CVarBoolean(bool init);
		};

		/**
		* @class snuffbox::engine::CVarNumber : public snuffbox::engine::CVarValue<float, snuffbox::engine::CVarBase::CVarTypes::kNumber>
		* @brief A CVar to store a numerical value represented as a float
		* @author Daniel Konings
		*/
		class CVarNumber : public CVarValue<float, CVarBase::CVarTypes::kNumber>
		{

			friend class CVar;
			friend class Allocator;

		protected:

			/**
			* @brief Default constructor, initialises the CVar
			* @param[in] init (float) The initial value
			*/
			CVarNumber(float init);

		public:

			/**
			* @brief Converts the float to a provided numerical value of type T
			* @return (T) The converted value
			*/
			template <typename T>
			T As() const;
		};

		//-----------------------------------------------------------------------------------------------
		template <typename T, int Y>
		CVarValue<T, Y>::CVarValue(const T& init) :
			value_(init)
		{

		}

		//-----------------------------------------------------------------------------------------------
		template <typename T, int Y>
		inline void CVarValue<T, Y>::set_value(const T& value)
		{
			value_ = value;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T, int Y>
		inline const T& CVarValue<T, Y>::value() const
		{
			return value_;
		}

		//-----------------------------------------------------------------------------------------------
		template <typename T>
		inline T CVarNumber::As() const
		{
			return static_cast<T>(value());
		}
	}
	
}
