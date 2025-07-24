# executable GLFW - specific 
if(USE_GLFW)
	file(GLOB_RECURSE HELLO_TRIANGLE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

	add_executable(hello_triangle ${HELLO_TRIANGLE_SOURCE})

	# Set the folder for Visual Studio
	set_target_properties(hello_triangle PROPERTIES FOLDER "test")

	# Include directories
	include_directories(${CMAKE_SOURCE_DIR}/externals/tsg/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/deps)

	# Set up source groups for Visual Studio filters
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source" FILES ${HELLO_TRIANGLE_SOURCE})

	# Linking engine libs
	target_link_libraries(hello_triangle PRIVATE engine)
	target_link_libraries(hello_triangle PRIVATE tsg)
	# Linking GLFW libs
	target_link_directories(hello_triangle PRIVATE "${CMAKE_SOURCE_DIR}/externals/GLFW/lib/")
	target_link_libraries(hello_triangle PRIVATE glfw3.lib)

	# Settings compiler options
	target_compile_features(hello_triangle PUBLIC cxx_std_23)
	if(MSVC)
		target_compile_options(hello_triangle PRIVATE /W4)
	else()
		target_compile_options(hello_triangle PRIVATE -Wall -Wextra)
	endif()

	add_custom_command(TARGET hello_triangle POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/assets
		$<TARGET_FILE_DIR:hello_triangle>/assets
		COMMENT "Copying shares files to build directory"
	)
endif()


