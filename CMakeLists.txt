cmake_minimum_required (VERSION 3.8)

find_package(OpenGL REQUIRED)

add_subdirectory ("deps/gl")
add_subdirectory ("deps/stb")
add_subdirectory ("deps/glfw")
add_subdirectory ("deps/imgui")
add_subdirectory ("dgnix")
