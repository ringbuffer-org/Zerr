# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.
cmake_minimum_required(VERSION 3.5)
set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)


#############################################################
# FIND ZERR LIBRARY - Required
#############################################################
# Specify the path to the libzerr.a static library
set(ZERR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../core/lib)
find_library(ZERR_LIBRARY zerr_core PATHS ${ZERR_LIB_DIR} NO_DEFAULT_PATH)

# Make the library a required dependency
if(NOT ZERR_LIBRARY)
    message(FATAL_ERROR "ERROR: libzerr_core.a is required but could not be found in ${ZERR_LIB_DIR}")
endif()

message(STATUS "Found libzerr_core.a: ${ZERR_LIBRARY}")

# Add include path for zerr headers
set(ZERR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../core/include)
if(NOT EXISTS ${ZERR_INCLUDE_DIR})
    message(FATAL_ERROR "ERROR: Zerr include directory not found at ${ZERR_INCLUDE_DIR}")
endif()

include_directories(${ZERR_INCLUDE_DIR}/features)
include_directories(${ZERR_INCLUDE_DIR}/modules)
include_directories(${ZERR_INCLUDE_DIR}/utils)

#############################################################
# MAX EXTERNAL
#############################################################
include_directories( 
    "${C74_INCLUDES}"
)
set(SOURCE_FILES
    ${PROJECT_NAME}.cpp
)
add_library( 
    ${PROJECT_NAME} 
    MODULE
    ${SOURCE_FILES}
)

# Link against the zerr library - required dependency
target_link_libraries(${PROJECT_NAME} PRIVATE ${ZERR_LIBRARY} yaml-cpp FFTW3::fftw3)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

#############################################################
# UNIT TEST
#############################################################
include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)