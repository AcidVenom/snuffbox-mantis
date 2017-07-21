#pragma once

namespace snuffbox
{
	namespace console
	{
		/**
		* @brief The different severities to log with
		*/
		enum struct LogSeverity : char
		{
			kDebug, //!< Used to log arbritrary debugging information
			kInfo, //!< Used to log info messages, e.g. initialisations or shutdowns
			kSuccess, //!< Used to log when something was succesful that should be succesful
			kWarning, //!< Used to log warnings that could potentially cause errors
			kError, //!< Used to log non-fatal errors, like JavaScript execution errors
			kFatal, //!< Used to log fatal errors, usually asserts
			kRGB, //!< Used to log something with your own specified colour
			kCount //!< The number of log severities
		};

		/**
		* @struct snuffbox::console::LogColour
		* @brief A structure to define a logging colour
		* @author Daniel Konings
		*/
		struct LogColour
		{
			/**
			* @struct snuffbox::LogColour::Colour
			* @brief A structure to define an RGB colour
			* @author Daniel Konings
			*/
			struct Colour
			{
				unsigned char r; //!< The red value
				unsigned char g; //!< The green value
				unsigned char b; //!< The blue value
			};

			Colour background; //!< The background colour of the text
			Colour foreground; //!< The foreground colour of the text
		};
	}
}