cmake_minimum_required (VERSION 3.8)

set(glfw_include_dir "include/")
set(glfw_libs_dir "libs/")

if(WIN32)
	set(lib_extension lib)
endif()

add_library(glfw_lib STATIC IMPORTED)
set_target_properties(glfw_lib PROPERTIES
			IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/${glfw_libs_dir}glfw3.${lib_extension}"
			IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/${glfw_libs_dir}glfw3d.${lib_extension}"
)

target_include_directories(glfw_lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/${glfw_include_dir}")

add_library (glfw_interface INTERFACE)
target_link_libraries(glfw_interface INTERFACE glfw_lib)
