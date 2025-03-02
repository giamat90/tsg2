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

# Build the engine first
add_subdirectory(engine)

# Build the tests
add_subdirectory(test)

# Build the tests
add_subdirectory(externals)

# test executable for glfw
add_executable(ConsoleApplication ConsoleApplication/ConsoleApplication.cpp)
target_link_directories(ConsoleApplication PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/externals/GLFW/lib/")
target_link_libraries(ConsoleApplication PRIVATE glfw3.lib)
target_include_directories(ConsoleApplication PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/externals/GLFW/include")
target_include_directories(ConsoleApplication PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/externals/tsg/include")
