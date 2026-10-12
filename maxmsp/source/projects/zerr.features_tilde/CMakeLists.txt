# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.
cmake_minimum_required(VERSION 3.5)
set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)


#############################################################
# FIND ZERR LIBRARY - Required (uses parent-defined paths)
#############################################################
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

# Link zerr_core and FFTW3 (spectral features). libzerr_core.a is found as a plain archive path, so its own
# dependencies do not propagate: every object names what it actually uses.
target_link_libraries(${PROJECT_NAME} PRIVATE ${ZERR_LIBRARY} FFTW3::fftw3)
target_compile_definitions(${PROJECT_NAME} PRIVATE ZERR_MAX_VERSION="${ZERR_MAX_PACKAGE_VERSION}")

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

#############################################################
# UNIT TEST
#############################################################
include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
