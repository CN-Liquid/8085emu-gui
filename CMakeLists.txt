cmake_minimum_required(VERSION 3.10)
project(8085emu-gui)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

file(GLOB SOURCES "modules/glfw/src/*.c")

add_executable(8085emu-gui   src/main.cpp
                                    src/imgui.cpp
                                    src/imgui_draw.cpp
                                    src/imgui_impl_opengl3.cpp
                                    src/imgui_widgets.cpp
                                    src/imgui_impl_glfw.cpp
                                    src/imgui_tables.cpp
                                    src/imgui_demo.cpp 
                                    src/8085_gui.cpp
					                          ${SOURCES}
)
if(UNIX AND NOT APPLE)  # Linux
    target_compile_definitions(8085emu-gui PRIVATE 
        "_GLFW_X11"
    )
elseif(WIN32)  # Windows
    target_compile_definitions(8085emu-gui PRIVATE 
        "_GLFW_WIN32"
    )
endif()


target_include_directories(8085emu-gui
  PRIVATE src
  PUBLIC include
  PUBLIC core/include
	PUBLIC modules/glfw/include
  PUBLIC modules/glfw/src
  PUBLIC ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(8085emu-gui
  PRIVATE ${CMAKE_SOURCE_DIR}/core/lib/lib8085.so
  PRIVATE ${CMAKE_SOURCE_DIR}/core/lib/libterminal.so
  PRIVATE ${OPENGL_LIBRARIES} 
    X11     
    pthread 
    dl 

)


