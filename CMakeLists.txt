cmake_minimum_required(VERSION 3.10)
project(8085emu-gui)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(OpenGL REQUIRED)



add_executable(8085emu-gui   src/main.cpp
                                    src/imgui.cpp
                                    src/imgui_draw.cpp
                                    src/imgui_impl_opengl3.cpp
                                    src/imgui_widgets.cpp
                                    src/imgui_impl_glfw.cpp
                                    src/imgui_tables.cpp
                                    src/imgui_demo.cpp 
                                    src/8085_gui.cpp
)
target_include_directories(8085emu-gui
  PRIVATE src
  PUBLIC include
  PUBLIC core/include
  PUBLIC ${OPENGL_INCLUDE_DIRS}
)
include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(8085emu-gui
  PRIVATE ${GLFW_LIBRARIES}
  PRIVATE ${CMAKE_SOURCE_DIR}/core/lib/lib8085.so
  PRIVATE ${CMAKE_SOURCE_DIR}/core/lib/libterminal.so
  PRIVATE ${OPENGL_LIBRARIES} 

)


