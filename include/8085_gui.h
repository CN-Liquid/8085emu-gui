#pragma once
#include "Terminal.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <functional>
#include <iostream>

class emu8085Gui {

protected:
  GLFWwindow *window;
  ImVec4 clear_color;
  ImGuiIO *io;
  terminal term;

public:
  emu8085Gui();

  ~emu8085Gui();

  void run();

  virtual void update() = 0;

  // virtual void startup() = 0;
};
