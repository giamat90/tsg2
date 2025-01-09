include(FetchContent)

# Fetch tsg from github
FetchContent_Declare(
	tsg
	GIT_REPOSITORY https://github.com/GiaMat90/tsg.git
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tsg
)
# Make the content available
FetchContent_MakeAvailable(tsg)

# Only configure SDL if it's enabled
if(USE_SDL)
    # Set SDL3 paths
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL/lib/debug")
    else()
        set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL/lib/release")
    endif()
    
    # Add SDL include directory
    set(SDL3_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/SDL/include")
    
    if(WIN32)
		# Create a custom target to copy SDL3 libraries
        add_custom_target(copy_sdl3_libs ALL
			# Copy SDL3 core
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_DIR}/SDL3.dll"
                "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_DIR}/SDL3.lib"
                "${CMAKE_BINARY_DIR}/lib"
			# Copy SDL3_image
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_ROOT}/SDL3_image.dll"
                "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_ROOT}/SDL3_image.lib"
                "${CMAKE_BINARY_DIR}/lib"
                
            # Copy SDL3_ttf
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_ROOT}/SDL3_ttf.dll"
                "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_ROOT}/SDL3_ttf.lib"
                "${CMAKE_BINARY_DIR}/lib"
            
            COMMENT "Copying SDL3 and extension libraries to build directory"
        )
        # For Windows, use the provided SDL3 libraries
        add_library(SDL3::SDL3 SHARED IMPORTED GLOBAL)
        set_target_properties(SDL3::SDL3 PROPERTIES
            IMPORTED_LOCATION "${SDL3_DIR}/SDL3.dll"
            IMPORTED_IMPLIB "${SDL3_DIR}/SDL3.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIRS}"
        )

        # SDL3_image
        add_library(SDL3::Image SHARED IMPORTED GLOBAL)
        set_target_properties(SDL3::Image PROPERTIES
            IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/bin/SDL3_image.dll"
            IMPORTED_IMPLIB "${CMAKE_BINARY_DIR}/lib/SDL3_image.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/SDL/include"
        )

        # SDL3_ttf
        add_library(SDL3::TTF SHARED IMPORTED GLOBAL)
        set_target_properties(SDL3::TTF PROPERTIES
            IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/bin/SDL3_ttf.dll"
            IMPORTED_IMPLIB "${CMAKE_BINARY_DIR}/lib/SDL3_ttf.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/SDL/include"
        )

        # Make all SDL3 targets depend on the copy operation
        add_dependencies(SDL3::SDL3 copy_sdl_libs)
        add_dependencies(SDL3::Image copy_sdl_libs)
        add_dependencies(SDL3::TTF copy_sdl_libs)
    else()
        # For Unix-like systems, try to find system SDL3 first
        find_package(SDL3 REQUIRED)
    endif()
endif()