cmake_minimum_required (VERSION 3.8)

set(gl_include 
"include/gl/gl_core_4_3.hpp")

set(gl_src 
"src/gl_core_4_3.cpp")

add_library (gl STATIC 
${gl_include}  
${gl_src})

target_compile_features (gl PRIVATE cxx_std_11)
target_include_directories(gl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(gl PRIVATE ${OPENGL_LIBRARY})
