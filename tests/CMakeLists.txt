cmake_minimum_required(VERSION 3.7.1)

project(WebService-test)

include(CRTLinkage)
include(PCHSupport)

#
# Source files.
#

set(TEST_HEADERS
)

set(TEST_SOURCES
	src/main
)

set(MISC
	src/pch.h
	src/pch.cpp
	include/targetver.h
	)
	
source_group("" FILES ${MISC})
source_group("Header Files" FILES ${TEST_HEADERS})
source_group("Source Files" FILES ${TEST_SOURCES})

include_directories(
	include
)

add_definitions(
	-D_SCL_SECURE_NO_WARNINGS
	-DUNICODE
	-D_UNICODE)

add_executable(WebService-test
	${TEST_HEADERS}
	${TEST_SOURCES}
	${MISC}
	)
add_precompiled_headers(WebService-test "src/")

target_link_libraries(WebService-test
)
