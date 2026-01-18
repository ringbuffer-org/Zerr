cmake_minimum_required(VERSION 3.19)

project(Zerr LANGUAGES CXX)

# Build core library first
add_subdirectory(core)

# Wrap Pure Data Makefile via CMake
add_subdirectory(puredata)

#
add_subdirectory(maxmsp)

# Ensure Pure Data build waits for core library
if(TARGET zerr_puredata AND TARGET zerr_core_static)
    add_dependencies(zerr_puredata zerr_core_static)
endif()

# Ensure Pure Data build waits for core library
if(TARGET zerr_maxmsp AND TARGET zerr_core_static)
    add_dependencies(zerr_maxmsp zerr_core_static)
endif()
