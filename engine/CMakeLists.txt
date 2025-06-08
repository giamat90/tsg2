# Add source files from engine directory (excluding sdl_port)
file(GLOB ENGINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*sdl_port/.*")

add_library(engine SHARED ${ENGINE_SOURCES})

# Set the folder for Visual Studio
set_target_properties(engine PROPERTIES FOLDER "tsg2")

source_group("sources" FILES ${ENGINE_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/externals/tsg/include)

# Linking tsg lib
target_link_libraries(engine PRIVATE tsg)

# add necessary simbols to export the dll
add_compile_definitions(TSG2_EXPORT)

if(USE_GLFW)
	# Add GLFW port sources to engine library
	file(GLOB GLFW_PORT_SOURCES 
		"${CMAKE_CURRENT_SOURCE_DIR}/glfw_port/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/glfw_port/*.h"
	)

	target_sources(engine PRIVATE ${GLFW_PORT_SOURCES})
	
	source_group("engine\\glfw_port" FILES ${GLFW_PORT_SOURCES})
	
	target_compile_definitions(engine PUBLIC USE_GLFW)

	# includes directories
	include_directories(${CMAKE_SOURCE_DIR}/externals/)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/deps)
	include_directories(${CMAKE_SOURCE_DIR}/externals/stb)

	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/externals/GLFW/lib/")
	target_link_libraries(engine PRIVATE glfw3.lib)
	
	add_compile_definitions(GLFW_INCLUDE_NONE)

	add_custom_command(TARGET engine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/glfw_port/shaders
		$<TARGET_FILE_DIR:engine>/shaders
		COMMENT "Copying shares files to build directory"
	)

endif()

if(USE_SDL)
	# Add SDL port sources to engine library
	file(GLOB SDL_PORT_SOURCES 
		"${CMAKE_CURRENT_SOURCE_DIR}/sdl_port/*.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/sdl_port/*.h"
	)

	target_sources(engine PRIVATE ${SDL_PORT_SOURCES})

	source_group("engine\\sdl_port" FILES ${SDL_PORT_SOURCES})

	target_compile_definitions(engine PUBLIC USE_SDL)

	# Add SDL link directory
	if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Active(Debug)")
		target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/externals/SDL/lib/debug")
	else()
		target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/externals/SDL/lib/release")
	endif()

	include_directories(${CMAKE_SOURCE_DIR}/externals/SDL/include)

	target_link_libraries(engine PRIVATE SDL3)
	target_link_libraries(engine PRIVATE SDL3_image)
	target_link_libraries(engine PRIVATE SDL3_ttf)
endif()


# Add include directories for engine
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
target_compile_features(engine PUBLIC cxx_std_20)

# Set appropriate compiler flags
if(MSVC)
    target_compile_options(engine PRIVATE /W4)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$(DEBUG_POSTFIX)")
	add_compile_definitions(UNICODE _UNICODE)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra)
endif()

