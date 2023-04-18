cmake_minimum_required (VERSION 3.8)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(dgnix_include_dir "include/")
set(dgnix_src_dir "src/")

set(dgnix_platforms_include_dir "platforms/include/gl/glfw")
set(dgnix_platforms_src_dir "platforms/src/gl/")

file(GLOB_RECURSE dgnix_include "${dgnix_include_dir}/*.hpp")
file(GLOB_RECURSE dgnix_include_private "${dgnix_src_dir}/*.hpp")
file(GLOB_RECURSE dgnix_src "${dgnix_src_dir}/*.cpp")

file(GLOB_RECURSE dgnix_platforms_include "${dgnix_platforms_include_dir}/*.hpp")
file(GLOB_RECURSE dgnix_platforms_include_private "${dgnix_platforms_src_dir}/glfw/*.hpp" "${dgnix_platforms_src_dir}/common/*.hpp")
file(GLOB_RECURSE dgnix_platforms_src "${dgnix_platforms_src_dir}/glfw/*.cpp" "${dgnix_platforms_src_dir}/common/*.cpp")

add_library (dgnix SHARED 
${dgnix_include}  
${dgnix_include_private}
${dgnix_src}
${dgnix_platforms_include}
${dgnix_platforms_include_private}
${dgnix_platforms_src})

target_compile_features (dgnix PRIVATE cxx_std_17)
target_include_directories(dgnix PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${dgnix_include_dir}" "${CMAKE_CURRENT_SOURCE_DIR}/${dgnix_platforms_include_dir}" 
								PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${dgnix_src_dir}")
target_link_libraries(dgnix PRIVATE glfw_interface PRIVATE glm_interface PRIVATE imgui_glfw PRIVATE gl PRIVATE stb PUBLIC math PUBLIC infrastruct)

install(TARGETS dgnix LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${dgnix_include} TYPE INCLUDE)
