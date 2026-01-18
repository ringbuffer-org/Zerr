cmake_minimum_required(VERSION 3.19)

project(zerr_maxmsp)

message(STATUS "------------- Setting up Zerr Max/MSP -------------")

enable_testing()

string(REGEX REPLACE "(.*)/" "" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")

if (APPLE)
    if (${CMAKE_GENERATOR} MATCHES "Xcode")
        if (${XCODE_VERSION} VERSION_LESS 10)
            message(STATUS "Xcode 10 or higher is required. Please install from the Mac App Store.")
            return ()
        elseif(${XCODE_VERSION} VERSION_GREATER_EQUAL 12)
            set(C74_BUILD_FAT YES)
        endif ()
    endif ()

    if (NOT CMAKE_OSX_ARCHITECTURES)
        if(C74_BUILD_FAT)
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
        else()
            set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
        endif()
        message("CMAKE_OSX_ARCHITECTURES set to ${CMAKE_OSX_ARCHITECTURES}")
    endif()

    # Set minimum macOS version to match the libraries
    if (NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "13.3" CACHE STRING "macOS deployment target" FORCE)
        message("CMAKE_OSX_DEPLOYMENT_TARGET set to ${CMAKE_OSX_DEPLOYMENT_TARGET}")
    endif()
endif()


# Misc setup and subroutines
include(${CMAKE_CURRENT_SOURCE_DIR}/source/min-api/script/min-package.cmake)


# Add the Lib, if it exists
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib)
endif ()


#############################################################
# Dependencies SETUP
#############################################################
# Reuse top-level targets if present; otherwise find them here.

# yaml-cpp (prefer config target from Conan/system)
if(NOT TARGET yaml-cpp::yaml-cpp)
    find_package(yaml-cpp 0.8 CONFIG REQUIRED)
endif()

# FFTW3 (accept either FFTW3::fftw3 or fftw::fftw; fallback to MODULE/vars)
if(NOT TARGET FFTW3::fftw3 AND NOT TARGET fftw::fftw AND NOT FFTW3_LIBRARIES)
    find_package(FFTW3 3.3 QUIET)
    if(NOT FFTW3_FOUND)
        find_package(FFTW3 3.3 MODULE REQUIRED)
    endif()
endif()

# Optional: expose zerr_core from the superbuild so subprojects can link it
if(TARGET zerr_core_static)
    add_library(zerr_core::zerr_core ALIAS zerr_core_static)
endif()

# Optional: a convenience INTERFACE target bundling deps for subprojects
add_library(zerr_deps INTERFACE)
target_link_libraries(zerr_deps INTERFACE yaml-cpp::yaml-cpp)

if(TARGET FFTW3::fftw3)
    target_link_libraries(zerr_deps INTERFACE FFTW3::fftw3)
elseif(TARGET fftw::fftw)
    target_link_libraries(zerr_deps INTERFACE fftw::fftw)
elseif(FFTW3_LIBRARIES)
    target_link_libraries(zerr_deps INTERFACE ${FFTW3_LIBRARIES})
    if(FFTW3_INCLUDE_DIRS)
        target_include_directories(zerr_deps INTERFACE ${FFTW3_INCLUDE_DIRS})
    endif()
endif()

if(TARGET zerr_core_static)
    target_link_libraries(zerr_deps INTERFACE zerr_core_static)
endif()

# Generate a project for every folder in the "source/projects" folder
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach (project_dir ${PROJECT_DIRS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
    endif ()
endforeach ()

# Comment the line below if you want automatic cmake regneration enabled
set(CMAKE_SUPPRESS_REGENERATION true)
