cmake_minimum_required(VERSION 3.15)

project(zerr_core VERSION 0.1.0 LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ============================================================================
# Dependency Management: Conan (default) or System Packages
# ============================================================================

option(USE_SYSTEM_DEPS "Force use of system packages, otherwise use conan packages" OFF)

# Check if Conan toolchain exists and suggest using it
if(NOT USE_SYSTEM_DEPS)
    set(DEFAULT_CONAN_TOOLCHAIN "build/conan_toolchain.cmake")

    if(NOT CMAKE_TOOLCHAIN_FILE AND EXISTS "${DEFAULT_CONAN_TOOLCHAIN}")
        message(STATUS "Found Conan toolchain at: ${DEFAULT_CONAN_TOOLCHAIN}")
        message(WARNING "CMAKE_TOOLCHAIN_FILE not set. For proper Conan builds, use:")
        message(WARNING "  cmake -B build -DCMAKE_TOOLCHAIN_FILE=${DEFAULT_CONAN_TOOLCHAIN}")
    elseif(NOT CMAKE_TOOLCHAIN_FILE AND NOT EXISTS "${DEFAULT_CONAN_TOOLCHAIN}")
        message(WARNING "Conan toolchain not found at ${DEFAULT_CONAN_TOOLCHAIN}")
        message(WARNING "Run first: conan install . --output-folder=build --build=missing")
    endif()
endif()

if(USE_SYSTEM_DEPS)
    if(APPLE)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt" "/opt/homebrew/opt" "/usr/local" "/usr")
    elseif(UNIX AND NOT APPLE)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local" "/usr" "/opt")
    elseif(WIN32)
        list(APPEND CMAKE_PREFIX_PATH "C:/Program Files" "C:/Program Files (x86)" "C:/vcpkg/installed")
    endif()
    message(STATUS "Using system packages from: ${CMAKE_PREFIX_PATH}")
endif()

macro(find_package_with_help PACKAGE_NAME MIN_VERSION)
    # Try CONFIG mode first (Conan packages)
    find_package(${PACKAGE_NAME} ${MIN_VERSION} CONFIG QUIET)

    # If not found and using system deps, try MODULE mode
    if(NOT ${PACKAGE_NAME}_FOUND AND USE_SYSTEM_DEPS)
        find_package(${PACKAGE_NAME} ${MIN_VERSION} MODULE QUIET)
    endif()

    # Check version if found
    if(${PACKAGE_NAME}_FOUND)
        if(${PACKAGE_NAME}_VERSION)
            message(STATUS "✓ Found ${PACKAGE_NAME} version ${${PACKAGE_NAME}_VERSION} (required: ${MIN_VERSION})")
        else()
            message(STATUS "✓ Found ${PACKAGE_NAME}")
        endif()
    else()
        # Not found anywhere
        message(FATAL_ERROR
            "Could not find ${PACKAGE_NAME} (>= ${MIN_VERSION}).\n\n"
            "For reproducible builds with Conan (recommended for CI/CD):\n"
            "  conan install . --output-folder=build --build=missing\n"
            "  cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake\n"
            "  cmake --build build\n\n"
            "For local development with system packages:\n"
            "  macOS (homebrew): brew install fftw yaml-cpp\n"
            "  Ubuntu/Debian: sudo apt-get install libfftw3-dev libyaml-cpp-dev\n"
            "  Windows (vcpkg): vcpkg install fftw3:x64-windows yaml-cpp:x64-windows\n"
            "  cmake -B build -DUSE_SYSTEM_DEPS=ON\n"
            "  cmake --build build\n\n"
            "Alternatively, specify the search path manually:\n"
            "  cmake -B build -DUSE_SYSTEM_DEPS=ON -DCMAKE_PREFIX_PATH=/path/to/packages\n"
        )
    endif()
endmacro()

# Find dependencies
find_package_with_help(yaml-cpp "0.8.0")
find_package_with_help(FFTW3 "3.3.10")

# Collect all source files
file(GLOB_RECURSE LIBZERRCORE_SRC
    src/utils/*.cpp
    src/modules/*.cpp
    src/features/*.cpp
)

# Create library
add_library(zerr_core_static STATIC ${LIBZERRCORE_SRC})

target_include_directories(zerr_core_static PUBLIC
    "include/utils"
    "include/modules"
    "include/features"
)

# Link yaml-cpp
target_link_libraries(zerr_core_static PUBLIC yaml-cpp::yaml-cpp)

# Link FFTW3 (try CMake target first, fallback to variables)
if(TARGET fftw::fftw)
    target_link_libraries(zerr_core_static PUBLIC fftw::fftw)
elseif(TARGET FFTW3::fftw3)
    target_link_libraries(zerr_core_static PUBLIC FFTW3::fftw3)
elseif(FFTW3_INCLUDE_DIRS AND FFTW3_LIBRARIES)
    target_include_directories(zerr_core_static PUBLIC ${FFTW3_INCLUDE_DIRS})
    target_link_directories(zerr_core_static PUBLIC ${FFTW3_LIBRARY_DIRS})
    target_link_libraries(zerr_core_static PUBLIC ${FFTW3_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find FFTW3 target or variables")
endif()

set_target_properties(zerr_core_static PROPERTIES
    OUTPUT_NAME "zerr_core"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Installation
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS zerr_core_static ARCHIVE DESTINATION lib)
