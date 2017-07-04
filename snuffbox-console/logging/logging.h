#pragma once

namespace snuffbox
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
}