SET (V8_ROOT_DIR "" CACHE PATH "The root directory of Google's V8")

FIND_PATH(V8_INCLUDE_DIR "v8.h"
	 PATHS
	 "${V8_ROOT_DIR}/include"
     "$ENV{V8_ROOT_DIR}/include"
)

SET (V8_LIBNAMES
	"v8_base_0"
	"v8_base_1"
	"v8_libbase"
	"v8_libplatform"
	"v8_libsampler"
	"v8_snapshot"
	"v8_nosnapshot"
	"icui18n"
	"icuuc"
)

FOREACH(V8_LIB ${V8_LIBNAMES})
	FIND_LIBRARY(V8_LIB_DEBUG_${V8_LIB}
		NAMES "${V8_LIB}"
		PATHS 
		"${V8_ROOT_DIR}/out.gn/x64.debug/obj"
		"$ENV{V8_ROOT_DIR}/out.gn/x64.debug/obj"
		"${V8_ROOT_DIR}/out.gn/x64.debug/obj/third_party/icu"
		"$ENV{V8_ROOT_DIR}/out.gn/x64.debug/obj/third_party/icu"
	)
	
	IF (V8_FIND_REQUIRED AND NOT V8_LIB_DEBUG_${V8_LIB})
		MESSAGE (FATAL_ERROR "Could not find ${V8_LIB} for x64.debug")
	ELSE ()
		SET (V8_LIBS_DEBUG ${V8_LIBS_DEBUG} ${V8_LIB_DEBUG_${V8_LIB}})
	ENDIF ()

	MARK_AS_ADVANCED(V8_LIB_DEBUG_${V8_LIB})
ENDFOREACH ()

FOREACH(V8_LIB ${V8_LIBNAMES})
	FIND_LIBRARY(V8_LIB_RELEASE_${V8_LIB}
		NAMES "${V8_LIB}"
		PATHS 
		"${V8_ROOT_DIR}/out.gn/x64.release/obj"
		"$ENV{V8_ROOT_DIR}/out.gn/x64.release/obj"
		"${V8_ROOT_DIR}/out.gn/x64.release/obj/third_party/icu"
		"$ENV{V8_ROOT_DIR}/out.gn/x64.release/obj/third_party/icu"
	)
	
	IF (V8_FIND_REQUIRED AND NOT V8_LIB_RELEASE_${V8_LIB})
		MESSAGE (FATAL_ERROR "Could not find ${V8_LIB} for x64.release")
	ELSE ()
		SET (V8_LIBS_RELEASE ${V8_LIBS_RELEASE} ${V8_LIB_RELEASE_${V8_LIB}})
	ENDIF ()

	MARK_AS_ADVANCED(V8_LIB_RELEASE_${V8_LIB})
ENDFOREACH ()

IF (V8_INCLUDE_DIR)
	SET (V8_FOUND TRUE)
	SET (V8_INCLUDE_DIRS ${V8_INCLUDE_DIR})

	IF (WIN32)
		SET(V8_WIN32_DEPS
			"winmm.lib"
			"dbghelp.lib"
			"shlwapi.lib"
		)
		SET(V8_LIBS_DEBUG ${V8_LIBS_DEBUG} ${V8_WIN32_DEPS})
		SET(V8_LIBS_RELEASE ${V8_LIBS_RELEASE} ${V8_WIN32_DEPS})
	ENDIF ()
	
	MESSAGE (STATUS "Found V8: ${V8_INCLUDE_DIRS}")
ELSE ()
	SET(V8_FOUND FALSE)
	IF (V8_FIND_REQUIRED)
		MESSAGE (FATAL_ERROR "Could not find V8")
	ENDIF ()
ENDIF ()