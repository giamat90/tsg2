file(GLOB_RECURSE PONG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(pong ${PONG_SOURCES})

# Set the folder for Visual Studio
set_target_properties(pong PROPERTIES FOLDER "test")

include_directories(${CMAKE_SOURCE_DIR}/externals/tsg/include)

# Set up source groups for Visual Studio filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source" FILES ${PONG_SOURCES})

target_link_libraries(pong PRIVATE engine)

target_compile_features(pong PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(pong PRIVATE /W4)
else()
    target_compile_options(pong PRIVATE -Wall -Wextra)
endif()

if(USE_SDL)

	# ToDo: remove this SDL3-dependecie
	if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Active(Debug)")
		target_link_directories(pong PRIVATE "${CMAKE_SOURCE_DIR}/externals/SDL/lib/debug")
	else()
		target_link_directories(pong PRIVATE "${CMAKE_SOURCE_DIR}/externals/SDL/lib/release")
	endif()
	include_directories(${CMAKE_SOURCE_DIR}/externals/SDL/include)
	target_link_libraries(pong PRIVATE SDL3)
	
	# necessary to run pong.exe
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SDL3_DIR "${CMAKE_SOURCE_DIR}/externals/SDL/lib/debug")
    else()
        set(SDL3_DIR "${CMAKE_SOURCE_DIR}/externals/SDL/lib/release")
    endif()
	add_custom_command(TARGET pong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_DIR}/SDL3.dll"
            "${SDL3_DIR}/SDL3_image.dll"
            "${SDL3_DIR}/SDL3_ttf.dll"
            $<TARGET_FILE_DIR:pong>
        COMMENT "Copying DLLs to pong output directory"
    )
endif()
