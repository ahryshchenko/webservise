cmake_minimum_required(VERSION 3.7.1)

project(WebService)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(PCHSupport)
include(DetectCXXCompiler)
include(CompilerOptions)
include(CRTLinkage)

# Set a default build type for single-configuration CMake generators if no build type is set.
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries (.dll) instead of static ones (.lib)" OFF)
option(BUILD_WITH_STATIC_CRT "Use static run-time libraries (/MT or /MTd linker flags)" ON)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_BUILD "0")

set(REVISION_COUNT "0")
set(VERSION_PREFIX "1")

find_package(Git)
if(GIT_FOUND)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-list HEAD --count
		OUTPUT_VARIABLE DESCRIBE_BUILD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_STRIP_TRAILING_WHITESPACE)
		string(REGEX MATCH "[0-9]+" REVISION_COUNT ${DESCRIBE_BUILD})
endif()

set(VERSION_REVISION "${VERSION_PREFIX}${REVISION_COUNT}")
set(VERSION_FILE_REVISION "${VERSION_PREFIX}${REVISION_COUNT}")

set(VERSION_COMPANY_NAME "Company Ltd")
set(VERSION_FILE_DESC "Web service")
set(VERSION_PRODUCT_NAME "WebService")
set(VERSION_COPYRIGHT "Copyright (C) 2016 ${VERSION_COMPANY_NAME}")

if(MSVC)
	# /bigobj is required for windows builds because of the size of some object files
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /bigobj")
endif()

#x32/x64 suffix(postfix)
if(CMAKE_CL_64)
	set(ARCH_SUFFIX "64")
else()
	set(ARCH_SUFFIX "")
endif()

#Deploy directory path
set(DEPLOY_PATH ${CMAKE_SOURCE_DIR}/deploy)

#Output directory path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Install location for external dependencies.
set(EXTERNAL_PATH "${CMAKE_SOURCE_DIR}/external")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Add libcurl to the project.
set(libcurl_INCLUDE_DIRS
	${EXTERNAL_PATH}/libcurl/include/)
set(libcurl_LIBRARIES
	debug ${EXTERNAL_PATH}/libcurl/lib/libcurld.lib
	optimized ${EXTERNAL_PATH}/libcurl/lib/libcurl.lib
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Set favor small code optimization
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Os ")
# Set full optimization
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ox ")

add_subdirectory(modules/WebService)