cmake_minimum_required(VERSION 3.7.1)

project(WebService)

include(CRTLinkage)
include(PCHSupport)

# Disable auto-linking to boost libraries.
add_definitions(-DBOOST_ALL_NO_LIB)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ${BUILD_WITH_STATIC_CRT})

find_package(Boost REQUIRED COMPONENTS filesystem system serialization date_time thread chrono program_options log)
include_directories(${Boost_INCLUDE_DIRS})

#
# Source files.
#

set(HEADER_FILES
	include/Log.h
	include/WebMonitor.h
	${JSONXX_PARSER_ROOT}/jsonxx.h
	)

set(SOURCE_FILES
	src/Log.cpp
	src/main.cpp
	src/WebMonitor.cpp
	${JSONXX_PARSER_ROOT}/jsonxx.cc
	)

set(MISC
	src/pch.h
	src/pch.cpp
	include/targetver.h
	)
		
source_group("" FILES ${MISC})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

include_directories(
	include
	${CMAKE_CURRENT_BINARY_DIR}
	${libcurl_INCLUDE_DIRS}
	${XML2JSON_INCLUDE_DIRS}
	${JSONXX_PARSER_ROOT})

add_definitions(
	-D_SCL_SECURE_NO_WARNINGS
	-DUNICODE
	-D_UNICODE
	-DCURL_STATICLIB)

add_executable(WebService
	${HEADER_FILES}
	${MISC}
	${SOURCE_FILES}
	)
add_precompiled_headers(WebService "src/")

set_target_properties(WebService PROPERTIES OUTPUT_NAME "WebService")

target_link_libraries(WebService
	Ws2_32.lib
	Wldap32.lib
	${Boost_LIBRARIES}
	${libcurl_LIBRARIES})

install(TARGETS WebService
		RUNTIME DESTINATION .)