file(GLOB_RECURSE HELLO_TRIANGLE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(hello_triangle ${HELLO_TRIANGLE_SOURCE})

# Set the folder for Visual Studio
set_target_properties(hello_triangle PROPERTIES FOLDER "test")

include_directories(${CMAKE_SOURCE_DIR}/externals/tsg)

# Set up source groups for Visual Studio filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source" FILES ${HELLO_TRIANGLE_SOURCE})

target_link_libraries(hello_triangle PRIVATE engine)

target_compile_features(hello_triangle PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(hello_triangle PRIVATE /W4)

else()
    target_compile_options(hello_triangle PRIVATE -Wall -Wextra)
endif()


