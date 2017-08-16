SET(GLSLang_ROOT_DIR "deps/glslang" CACHE STRING "The root directory for the source of GLSLang")

FIND_FILE(GLSLang_INCLUDE
	NAMES "ShaderLang.h"
	PATHS
	"${GLSLang_ROOT_DIR}/glslang/Public"
)

IF (GLSLang_INCLUDE)
	SET (GLSLang_FOUND TRUE)
	
	SET (GLSLang_INCLUDE_DIRECTORIES
		"${GLSLang_ROOT_DIR}/glslang/Include"
		"${GLSLang_ROOT_DIR}/glslang/Public"
		"${GLSLang_ROOT_DIR}/glslang/OSDependent"
		"${GLSLang_ROOT_DIR}"
	)
	
	MESSAGE (STATUS "Found GLSLang: ${GLSLang_INCLUDE}")
ELSE ()
	SET (GLSLang_FOUND FALSE)
	IF (GLSLang_FIND_REQUIRED)
		MESSAGE (FATAL_ERROR "Could not find GLSLang")
	ENDIF ()
ENDIF ()
