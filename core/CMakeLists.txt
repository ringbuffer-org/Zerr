cmake_minimum_required(VERSION 3.15)

project(zerr_core VERSION 0.1.0 LANGUAGES CXX)

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

target_include_directories(zerr_core_static PUBLIC "include/utils")
target_include_directories(zerr_core_static PUBLIC "include/modules")
target_include_directories(zerr_core_static PUBLIC "include/features")

target_link_libraries(zerr_core_static PUBLIC yaml-cpp FFTW3::fftw3)

set_target_properties(zerr_core_static PROPERTIES OUTPUT_NAME "zerr_core")

get_target_property(OUTPUT_VALUE zerr_core_static OUTPUT_NAME)
message(STATUS "This is the zerr_core_static OUTPUT_NAME: " ${OUTPUT_VALUE})

# Set installation path relative to the project directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Install the static library into the local "lib" folder 
install(TARGETS zerr_core_static ARCHIVE DESTINATION lib)
# install(DIRECTORY include/ DESTINATION usr/include)