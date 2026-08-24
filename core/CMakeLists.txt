cmake_minimum_required(VERSION 3.15)

project(zerr_core VERSION 0.1.0 LANGUAGES CXX)

add_definitions(-DYAML_CPP_STATIC_DEFINE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Emit build/compile_commands.json, so clangd can resolve the core's own sources.
# Its three include directories and the conan dependency paths are known only
# here; without the database an editor reports every #include as missing. clangd
# finds it unaided -- it searches the "build" subdirectory of each ancestor of
# the file being edited -- but it only exists once the target is configured.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pin the macOS deployment target, matching maxmsp/CMakeLists.txt's approach.
#
# Without this the core compiles against the host SDK while the wrappers link at
# a much older floor -- measured before the fix: libzerr_core.a at `minos 15.0`
# inside a zerr_features~.pd_darwin declaring `minos 11.0`. Nothing in the core
# uses availability-gated libc++ today (no <filesystem>, <charconv>, <format>),
# so that was latent rather than broken -- but at a 15.0 target the compiler would
# silently accept a newer-only API and ship an external that fails on older macs.
#
# 10.13 is the project's single declared floor: it is what maxmsp/CMakeLists.txt
# pins and what the PureData macOS CI job passes as -mmacosx-version-min. Do not
# raise it here in isolation -- on arm64 clang clamps any lower value up to 11.0
# (arm64's own floor), so 10.13 and 11.0 are indistinguishable on Apple Silicon,
# but on an Intel or fat build a core pinned to 11.0 against wrappers at 10.13
# reintroduces the very object-version mismatch this is meant to remove, just in
# the opposite direction. One floor, stated in each place that needs it.
#
# Deliberately set here rather than via `os.version` in the Conan profile:
# os.version is part of the package_id, so pinning it there means no ConanCenter
# prebuilt ever matches and every dependency is rebuilt from source, in CI too.
# See profiles/macos and docs/design/repo-audit-2026-07-30.md.
if(APPLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "macOS deployment target" FORCE)
    message(STATUS "CMAKE_OSX_DEPLOYMENT_TARGET set to ${CMAKE_OSX_DEPLOYMENT_TARGET}")
endif()

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
