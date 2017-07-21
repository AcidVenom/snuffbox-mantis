CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(snuffbox-mantis C CXX)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake-targets")

SET(SNUFF_DEFAULT_PORT "8888" CACHE STRING "Specifies the default port for the logging connection to use")
SET(SNUFF_LOG_TIMEOUT "3" CACHE STRING "Specifies the default timeout in seconds for the logging connection to use")
SET(SNUFF_LOG_BUFFERSIZE "4096" CACHE STRING "Specifies the maximum buffer size for the logging connection to use per socket")
OPTION(SNUFF_USE_OGL "Forces OpenGL or Vulkan on Windows")
SET(SNUFF_DIRECTX_VERSION "11" CACHE STRING "Specifies the DirectX version to use")
SET(SNUFF_OGL_VERSION "ogl" CACHE STRING "Specifies whether to use OpenGL or Vulkan")
SET_PROPERTY(CACHE "SNUFF_DIRECTX_VERSION" PROPERTY STRINGS "11" "12")
SET_PROPERTY(CACHE "SNUFF_OGL_VERSION" PROPERTY STRINGS "ogl" "vulkan")
SET(SNUFF_RELOAD_AFTER "1000" CACHE STRING "Specifies after how many frames the file watcher should reload")
OPTION(SNUFF_BUILD_TEST "Build the test environment project 'snuffbox-test'")

OPTION(SNUFF_JAVASCRIPT "Should V8 (JavaScript) be enabled for this build?")

IF (WIN32)
	FOREACH(FLAG CMAKE_C_FLAGS
	             CMAKE_C_FLAGS_DEBUG
	             CMAKE_C_FLAGS_RELEASE
	             CMAKE_CXX_FLAGS
	             CMAKE_CXX_FLAGS_DEBUG
	             CMAKE_CXX_FLAGS_RELEASE)

	    IF (${FLAG} MATCHES "/MD")
	        STRING(REGEX REPLACE "/MD" "/MT" ${FLAG} "${${FLAG}}")
	    ENDIF ()

	    IF (${FLAG} MATCHES "/MDd")
	        STRING(REGEX REPLACE "/MDd" "/MTd" ${FLAG} "${${FLAG}}")
	    ENDIF ()

    ENDFOREACH()
ENDIF ()

SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:Debug>:SNUFF_DEBUG>
    $<$<CONFIG:Release>:SNUFF_RELEASE>
)

IF (WIN32)
	ADD_DEFINITIONS("/DSNUFF_WIN32")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	ADD_DEFINITIONS("/DSNUFF_LINUX")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fno-rtti")
ENDIF ()

ADD_DEFINITIONS("/DSNUFF_DEFAULT_PORT=${SNUFF_DEFAULT_PORT}")
ADD_DEFINITIONS("/DSNUFF_LOG_TIMEOUT=${SNUFF_LOG_TIMEOUT}")
ADD_DEFINITIONS("/DSNUFF_LOG_BUFFERSIZE=${SNUFF_LOG_BUFFERSIZE}")
ADD_DEFINITIONS("/DSNUFF_RELOAD_AFTER=${SNUFF_RELOAD_AFTER}")

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

FIND_PACKAGE(EASTL REQUIRED)
ADD_SUBDIRECTORY("${EASTL_ROOT_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/deps/eastl")
INCLUDE_DIRECTORIES("${EASTL_DIR}/include")

SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW docs")
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests")
SET(GLFW_INSTALL OFF CACHE BOOL "Install GLFW")

FIND_PACKAGE(GLFWSource REQUIRED)
ADD_SUBDIRECTORY("${GLFW_ROOT_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/deps/glfw")

IF (WIN32)
	SET (wxWidgets_ROOT_DIR "deps/wxWidgets" CACHE PATH "The root directory of wxWidgets")
ENDIF ()

FIND_PACKAGE(wxWidgets REQUIRED COMPONENTS core base richtext html qa xrc propgrid aui adv xml)
INCLUDE_DIRECTORIES("${wxWidgets_ROOT_DIR}/include")

IF (WIN32)
	INCLUDE_DIRECTORIES(
		$<$<CONFIG:Debug>:${wxWidgets_LIB_DIR}/mswud>
		$<$<CONFIG:Release>:${wxWidgets_LIB_DIR}/mswu>
	)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	INCLUDE_DIRECTORIES(
		$<$<CONFIG:Debug>:${wxWidgets_LIB_DIR}/wx/include/gtk3-unicode-static-${WX_VERSION}>
		$<$<CONFIG:Release>:${wxWidgets_LIB_DIR}/wx/include/gtk3-unicode-static-${WX_VERSION}>
	)
ENDIF ()

IF (SNUFF_JAVASCRIPT)
        ADD_DEFINITIONS("/DSNUFF_JAVASCRIPT")
        ADD_DEFINITIONS("/DV8_IMMINENT_DEPRECATION_WARNINGS")

	FIND_PACKAGE(V8 REQUIRED)

	INCLUDE_DIRECTORIES(${V8_INCLUDE_DIR})
ENDIF ()

ADD_SUBDIRECTORY("snuffbox-logging")
ADD_SUBDIRECTORY("snuffbox-console")
ADD_SUBDIRECTORY("snuffbox-engine")

IF (SNUFF_BUILD_TEST)
	ADD_SUBDIRECTORY("snuffbox-test")
	SET_TARGET_PROPERTIES(snuffbox-test PROPERTIES FOLDER "snuffbox-mantis")
ENDIF ()

IF (SNUFF_JAVASCRIPT)
	ADD_SUBDIRECTORY("snuffbox-javascript")
	SET_TARGET_PROPERTIES(snuffbox-javascript PROPERTIES FOLDER "snuffbox-mantis")
ENDIF ()

SET (GLFWTests
	"clipboard"
	"cursor"
	"empty"
	"events"
	"gamma"
	"glfwinfo"
	"icon"
	"iconify"
	"joysticks"
	"monitors"
	"msaa"
	"reopen"
	"sharing"
	"tearing"
	"threads"
	"timeout"
	"title"
	"windows"
)

SET (GLFWExamples
	"boing"
	"gears"
	"heightmap"
	"particles"
	"simple"
	"splitview"
	"wave"
)

SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "deps/glfw3")

IF (GLFW_INSTALL)
	SET_TARGET_PROPERTIES(uninstall PROPERTIES FOLDER "deps/glfw3")
ENDIF ()

IF (GLFW_BUILD_EXAMPLES)
	FOREACH (GLFWName ${GLFWExamples})
		SET_TARGET_PROPERTIES(${GLFWName} PROPERTIES FOLDER "deps/glfw3/examples")
	ENDFOREACH ()
ENDIF ()

IF (GLFW_BUILD_TESTS)
	FOREACH (GLFWName ${GLFWTests})
		SET_TARGET_PROPERTIES(${GLFWName} PROPERTIES FOLDER "deps/glfw3/tests")
	ENDFOREACH ()
ENDIF ()

SET_TARGET_PROPERTIES(EASTL PROPERTIES FOLDER "deps/eastl")
SET_TARGET_PROPERTIES(EABase_ide PROPERTIES FOLDER "deps/eastl")
SET_TARGET_PROPERTIES(snuffbox-logging PROPERTIES FOLDER "snuffbox-mantis")
SET_TARGET_PROPERTIES(snuffbox-console PROPERTIES FOLDER "snuffbox-mantis")
SET_TARGET_PROPERTIES(snuffbox-engine PROPERTIES FOLDER "snuffbox-mantis")
