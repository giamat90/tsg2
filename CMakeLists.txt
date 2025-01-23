cmake_minimum_required(VERSION 3.12)
project(tsg2)

# Enable grouping targets into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Option to enable/disable SDL
option(USE_SDL "Enable SDL3 support" OFF)
option(USE_GLFW "Enable GLFW support" ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configure SDL3 at the top level if enabled
if(USE_SDL)
    # Setup SDL3 paths
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SDL3_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tsg2/externals/SDL/lib/debug")
    else()
        set(SDL3_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tsg2/externals/SDL/lib/release")
    endif()
    
    # Add SDL include directory to search path
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tsg2/externals/SDL")
    
    if(WIN32)
        # For Windows, create imported target for SDL3
        add_library(SDL3::SDL3 SHARED IMPORTED GLOBAL)
        set_target_properties(SDL3::SDL3 PROPERTIES
            IMPORTED_LOCATION "${SDL3_ROOT}/SDL3.dll"
            IMPORTED_IMPLIB "${SDL3_ROOT}/SDL3.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/tsg2/externals/SDL/include"
        )
    else()
        # For Unix-like systems, try to find system SDL3
        find_package(SDL3 REQUIRED)
    endif()
endif()

# Build the engine first
add_subdirectory(engine)

# Build the tests
add_subdirectory(test)
