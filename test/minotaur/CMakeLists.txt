# executable GLFW - specific 
if(USE_GLFW)
	file(GLOB_RECURSE MINOTAUR_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

	add_executable(minotaur ${MINOTAUR_SOURCE})

	# Set the folder for Visual Studio
	set_target_properties(minotaur PROPERTIES FOLDER "test")

	# Include directories
	include_directories(${CMAKE_SOURCE_DIR}/externals/)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/deps)
	include_directories(${CMAKE_SOURCE_DIR}/externals/stb)

	target_link_directories(minotaur PRIVATE "${CMAKE_SOURCE_DIR}/externals/GLFW/lib/")
	target_link_libraries(minotaur PRIVATE glfw3.lib)
	
	# Set up source groups for Visual Studio filters
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source" FILES ${MINOTAUR_SOURCE})

	# Linking GLFW libs
	target_link_directories(minotaur PRIVATE "${CMAKE_SOURCE_DIR}/externals/GLFW/lib/")
	target_link_libraries(minotaur PRIVATE glfw3.lib)

	# Settings compiler options
	target_compile_features(minotaur PUBLIC cxx_std_20)
	if(MSVC)
		target_compile_options(minotaur PRIVATE /W4)
	else()
		target_compile_options(minotaur PRIVATE -Wall -Wextra)
	endif()
endif()


