cmake_minimum_required(VERSION 3.15)

project(zerr_jack VERSION 0.1.0 LANGUAGES CXX)

# yaml-cpp is linked statically through libzerr_core.a; without this its headers
# declare the symbols dllimport-style. Matches core/CMakeLists.txt.
add_definitions(-DYAML_CPP_STATIC_DEFINE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emit build/compile_commands.json. The include paths this target needs -- three
# core header directories plus the conan and pkg-config ones -- are known only to
# the build system, so without a compilation database clangd cannot resolve a
# single #include in this target and reports every symbol as undefined.
#
# No .clangd file is needed alongside it: clangd searches each ancestor directory
# of a source file and the "build" subdirectory of each, which is exactly where
# this lands. It does have to exist, though -- configure the target once (any of
# `./build.sh jack`, `-i`, or a bare `cmake -S jack -B jack/build`) before
# expecting the editor to resolve anything, and note that `./build.sh -c jack`
# removes it again along with the rest of the build tree.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# No CMAKE_OSX_DEPLOYMENT_TARGET pin here, unlike core/ and maxmsp/.
#
# That pin exists so a loadable external and the core archive inside it agree on
# one floor. This target is not an external: it is an executable that links
# libjack as a shared library, and libjack comes from the machine's own package
# manager -- Homebrew builds it against the current SDK. Declaring 10.13 makes
# the linker warn ("building for macOS-11.0, but linking with dylib ... built for
# newer version 15.0") and puts a floor on the binary that the dylib it needs
# does not honour anyway. Linking the 10.13 libzerr_core.a into a host-target
# executable is the harmless direction and warns about nothing.

#############################################################
# ZERR CORE LIBRARY SETUP
#############################################################
# Same discovery the Max/MSP target uses: the archive the core install step drops
# into core/lib, overridable for out-of-tree layouts.
set(ZERR_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../core/lib" CACHE PATH "Path to zerr_core library directory")
set(ZERR_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../core/include" CACHE PATH "Path to zerr_core include directory")

find_library(ZERR_LIBRARY zerr_core PATHS ${ZERR_LIB_DIR} NO_DEFAULT_PATH)
if(NOT ZERR_LIBRARY)
    message(FATAL_ERROR "ERROR: libzerr_core.a is required but could not be found in ${ZERR_LIB_DIR}. Build it first: ./build.sh core")
endif()
message(STATUS "Found libzerr_core.a: ${ZERR_LIBRARY}")

if(NOT EXISTS ${ZERR_INCLUDE_DIR})
    message(FATAL_ERROR "ERROR: Zerr include directory not found at ${ZERR_INCLUDE_DIR}")
endif()

#############################################################
# Dependencies SETUP
#############################################################
# fftw3 and yaml-cpp come from the shared conan resolve at the repo root, the same
# one core/ and maxmsp/ consume -- configure this target with
# -DCMAKE_TOOLCHAIN_FILE=<repo>/build/conan_toolchain.cmake.
find_package(FFTW3 REQUIRED)
find_package(yaml-cpp REQUIRED)

# JACK is deliberately not a conan package: the client has to link the same
# libjack the running server was built against, so it is resolved from the system
# through pkg-config (jack.pc ships with both JACK1 and JACK2).
find_package(PkgConfig REQUIRED)
pkg_check_modules(JACK REQUIRED IMPORTED_TARGET jack)
message(STATUS "Found JACK ${JACK_VERSION}: ${JACK_LINK_LIBRARIES}")

#############################################################
# TARGET
#############################################################
add_executable(zerr_jack
    src/main.cpp
    src/zerr_jack.cpp
)

target_include_directories(zerr_jack PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ZERR_INCLUDE_DIR}/utils
    ${ZERR_INCLUDE_DIR}/modules
    ${ZERR_INCLUDE_DIR}/features
)

target_link_libraries(zerr_jack PRIVATE
    ${ZERR_LIBRARY}
    yaml-cpp
    FFTW3::fftw3
    PkgConfig::JACK
)

# Install next to the sources by default, like core/ does, so a plain
# `cmake --install` never writes outside the repo.
if(NOT DEFINED CACHE{CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT_ZERR_JACK})
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "Install prefix" FORCE)
    set(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT_ZERR_JACK TRUE CACHE INTERNAL "")
endif()

install(TARGETS zerr_jack RUNTIME DESTINATION bin)
