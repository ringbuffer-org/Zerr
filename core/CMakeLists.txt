cmake_minimum_required(VERSION 3.15)

project(zerr_core VERSION 0.1.0 LANGUAGES CXX)

add_definitions(-DYAML_CPP_STATIC_DEFINE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# find packages
find_package(FFTW3 REQUIRED)
find_package(yaml-cpp REQUIRED)

# Collect all source files
file(GLOB_RECURSE LIBZERRCORE_SRC
    src/utils/*.cpp
    src/modules/*.cpp
    src/features/*.cpp
)

add_library(zerr_core_static STATIC ${LIBZERRCORE_SRC})

target_include_directories(zerr_core_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/utils>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/modules>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/features>
)

target_link_libraries(zerr_core_static PUBLIC yaml-cpp FFTW3::fftw3)

set_target_properties(zerr_core_static PROPERTIES
    OUTPUT_NAME "zerr_core"
    EXPORT_NAME "zerr_core"
)

get_target_property(OUTPUT_VALUE zerr_core_static OUTPUT_NAME)
message(STATUS "This is the zerr_core_static OUTPUT_NAME: " ${OUTPUT_VALUE})

# Set installation path relative to the project directory (overridable via -DCMAKE_INSTALL_PREFIX=...)
if(NOT DEFINED CACHE{CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT_ZERR})
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "Install prefix" FORCE)
    set(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT_ZERR TRUE CACHE INTERNAL "")
endif()

# Install the static library into the local "lib" folder
install(TARGETS zerr_core_static
    EXPORT zerr_core_targets
    ARCHIVE DESTINATION lib
)

# Export CMake package config so downstream can use find_package(zerr_core)
install(EXPORT zerr_core_targets
    FILE zerr_core-targets.cmake
    NAMESPACE zerr::
    DESTINATION lib/cmake/zerr_core
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/zerr_core-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/zerr_core-config.cmake"
    INSTALL_DESTINATION lib/cmake/zerr_core
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/zerr_core-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/zerr_core-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/zerr_core-config-version.cmake"
    DESTINATION lib/cmake/zerr_core
)
