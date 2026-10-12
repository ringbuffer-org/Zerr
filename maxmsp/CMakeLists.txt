cmake_minimum_required(VERSION 3.19)
project(Min-DevKit)

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
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "macOS deployment target" FORCE)
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
# ZERR CORE LIBRARY SETUP
#############################################################
# Centralized paths to zerr core library (overridable via -DZERR_LIB_DIR=... -DZERR_INCLUDE_DIR=...)
set(ZERR_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../core/lib" CACHE PATH "Path to zerr_core library directory")
set(ZERR_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../core/include" CACHE PATH "Path to zerr_core include directory")

find_library(ZERR_LIBRARY zerr_core PATHS ${ZERR_LIB_DIR} NO_DEFAULT_PATH)
if(NOT ZERR_LIBRARY)
    message(FATAL_ERROR "ERROR: libzerr_core.a is required but could not be found in ${ZERR_LIB_DIR}")
endif()
message(STATUS "Found libzerr_core.a: ${ZERR_LIBRARY}")

if(NOT EXISTS ${ZERR_INCLUDE_DIR})
    message(FATAL_ERROR "ERROR: Zerr include directory not found at ${ZERR_INCLUDE_DIR}")
endif()

#############################################################
# Dependencies SETUP
#############################################################
find_package(FFTW3 REQUIRED)
find_package(yaml-cpp REQUIRED)

#############################################################
# PACKAGE VERSION
#############################################################
# package-info.json is the single source of truth for the version: the objects
# report it from `bang` and the zip produced by `cmake --install` carries it.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/package-info.json" ZERR_MAX_PACKAGE_INFO)
string(JSON ZERR_MAX_PACKAGE_VERSION GET "${ZERR_MAX_PACKAGE_INFO}" version)
message(STATUS "Zerr Max package version: ${ZERR_MAX_PACKAGE_VERSION}")

# Generate a project for every folder in the "source/projects" folder
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
set(ZERR_MAX_EXTERNALS "")
foreach (project_dir ${PROJECT_DIRS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
        # Same rule max-pretarget.cmake applies to derive the bundle name.
        string(REGEX REPLACE "_tilde$" "~" extern_name "${project_dir}")
        list(APPEND ZERR_MAX_EXTERNALS "${extern_name}")
    endif ()
endforeach ()

#############################################################
# PACKAGE ASSEMBLY  (cmake --install maxmsp/build)
#############################################################
# Assembles a Max package folder that drops straight into ~/Documents/Max 9/Packages.
# The folder name is the package name Max shows, so the prefix ends in /Zerr.
# Overridable with -DCMAKE_INSTALL_PREFIX=...; the cache guard is the idiom
# core/CMakeLists.txt uses, so a user-supplied prefix is never overwritten.
if(NOT DEFINED CACHE{ZERR_MAX_INSTALL_PREFIX_INITIALIZED})
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/dist/Zerr" CACHE PATH "Max package install location" FORCE)
    set(ZERR_MAX_INSTALL_PREFIX_INITIALIZED TRUE CACHE INTERNAL "")
endif()

# Only the bundles this tree builds, so a stale bundle left in externals/ from an
# old object name never ends up in the package. Copying the bundle directory keeps
# the ad-hoc signature max-posttarget.cmake applied; CI verifies it after install.
foreach (extern_name ${ZERR_MAX_EXTERNALS})
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/externals/${extern_name}.mxo"
            DESTINATION externals USE_SOURCE_PERMISSIONS)
endforeach ()
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/help/" DESTINATION help
        FILES_MATCHING PATTERN "*.maxhelp" PATTERN "*.yaml" PATTERN "*.mxo" EXCLUDE)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/extras/" DESTINATION extras
        FILES_MATCHING PATTERN "*.maxpat")
# Speaker layouts shipped with the package. Max adds a package's misc/ folder to
# the search path, so `zerr.envelopes~ trigger ring_8` resolves without a path.
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../configs/" DESTINATION misc
        FILES_MATCHING PATTERN "*.yaml")
install(FILES package-info.json README.md LICENSE DESTINATION .)

# Hand-copied bundles in help/ shadow fresh builds (Max loads whichever it finds
# first) and would duplicate every object if help/ were shipped as-is.
file(GLOB ZERR_MAX_STALE_HELP_BUNDLES "${CMAKE_CURRENT_SOURCE_DIR}/help/*.mxo")
if(ZERR_MAX_STALE_HELP_BUNDLES)
    message(WARNING "maxmsp/help/ contains .mxo bundles that are not part of the build and shadow "
                    "fresh externals: ${ZERR_MAX_STALE_HELP_BUNDLES}. Delete them (./build.sh -c maxmsp "
                    "does) and test from the installed package in maxmsp/dist/Zerr instead.")
endif()

# Comment the line below if you want automatic cmake regneration enabled
set(CMAKE_SUPPRESS_REGENERATION true)
