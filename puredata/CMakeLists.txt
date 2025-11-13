cmake_minimum_required(VERSION 3.19)

project(zerr_puredata)

# Custom target that calls the Makefile
add_custom_target(zerr_puredata ALL
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building Zerr* for Pure Data"
)

# Conditionally depend on core if built in a superproject
if(TARGET zerr_core_static)
    add_dependencies(zerr_puredata zerr_core_static)
endif()

# Convenience targets for clean and install via the Makefile
add_custom_target(zerr_puredata_clean
    COMMAND make clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Cleaning Zerr* for Pure Data"
)

add_custom_target(zerr_puredata_install
    COMMAND make install
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Installing Zerr* for Pure Data"
)
