cmake_minimum_required (VERSION 3.8)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(imgui_include 
"imgui/imconfig.h"
"imgui/imgui.h"
"imgui/imgui_internal.h"
"imgui/imstb_rectpack.h"
"imgui/imstb_textedit.h"
"imgui/imstb_truetype.h")

set(imgui_src 
"imgui/imgui.cpp"
"imgui/imgui_demo.cpp"
"imgui/imgui_draw.cpp"
"imgui/imgui_tables.cpp"
"imgui/imgui_widgets.cpp")

add_library (imgui SHARED 
${imgui_include}  
${imgui_src})

target_compile_features (imgui PRIVATE cxx_std_11)
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/")
target_link_libraries(imgui PRIVATE ${CMAKE_DL_LIBS})

install(TARGETS imgui LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${imgui_include} TYPE INCLUDE)



set(imgui_gl_include 
"imgui/backends/imgui_impl_opengl3.h"
"imgui/backends/imgui_impl_opengl3_loader.h")

set(imgui_gl_src 
"imgui/backends/imgui_impl_opengl3.cpp")

add_library (imgui_gl STATIC 
${imgui_gl_include}  
${imgui_gl_src})

target_compile_features (imgui_gl PRIVATE cxx_std_11)
target_include_directories(imgui_gl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/")
target_link_libraries(imgui_gl PUBLIC imgui PRIVATE ${OPENGL_LIBRARY} PRIVATE ${CMAKE_DL_LIBS})



set(imgui_glfw_include 
"imgui/backends/imgui_impl_glfw.h")

set(imgui_glfw_src 
"imgui/backends/imgui_impl_glfw.cpp")

add_library (imgui_glfw STATIC 
${imgui_glfw_include}  
${imgui_glfw_src})

target_compile_features (imgui_glfw PRIVATE cxx_std_11)
target_include_directories(imgui_glfw PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/")
target_link_libraries(imgui_glfw PUBLIC imgui_gl PRIVATE glfw_interface PRIVATE ${OPENGL_LIBRARY} PRIVATE ${CMAKE_DL_LIBS})
