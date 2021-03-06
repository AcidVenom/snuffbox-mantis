FILE(GLOB SNUFF_LOGGING_ROOT
	"*.cc"
	"*.h"
)

FILE(GLOB SNUFF_LOGGING_CONNECTION
	"connection/*.cc"
	"connection/*.h"
)

SOURCE_GROUP("\\" FILES ${SNUFF_LOGGING_ROOT})
SOURCE_GROUP("connection" FILES ${SNUFF_LOGGING_CONNECTION})

SET(SNUFF_LOGGING_SOURCES
	${SNUFF_LOGGING_ROOT}
	${SNUFF_LOGGING_CONNECTION}
)

IF (WIN32)
	FILE(GLOB SNUFF_LOGGING_WIN32
		"win32/*.cc"
		"win32/*.h"
	)

	SOURCE_GROUP("win32" FILES ${SNUFF_LOGGING_WIN32})
	SET(SNUFF_LOGGING_SOURCES ${SNUFF_LOGGING_SOURCES} ${SNUFF_LOGGING_WIN32})
ENDIF ()

ADD_LIBRARY(snuffbox-logging ${SNUFF_LOGGING_SOURCES})

IF (WIN32)
	TARGET_LINK_LIBRARIES(snuffbox-logging "WINMM.lib" "ws2_32.lib")
ENDIF ()