SET(SpirVCross_ROOT_DIR "deps/spirv-cross" CACHE STRING "The root directory for the source of SpirVCross")

FIND_FILE(SpirVCross_INCLUDE
	NAMES "spirv.hpp"
	PATHS
	"${SpirVCross_ROOT_DIR}"
)

IF (SpirVCross_INCLUDE)
	SET (SpirVCross_FOUND TRUE)
	SET (SpirVCross_INCLUDE_DIRECTORIES "${SpirVCross_ROOT_DIR}/include")
	
	MESSAGE (STATUS "Found SpirVCross: ${SpirVCross_INCLUDE}")
ELSE ()
	SET (SpirVCross_FOUND FALSE)
	IF (SpirVCross_FIND_REQUIRED)
		MESSAGE (FATAL_ERROR "Could not find SpirVCross")
	ENDIF ()
ENDIF ()
