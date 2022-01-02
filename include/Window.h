/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* Minimal window class
*/

#ifndef WINDOW_H
#define WINDOW_H 1


#include <cstdint>
#include <GLFW/glfw3.h>

constexpr uint32_t DEFAULT_WIDTH = 800;
constexpr uint32_t DEFAULT_HEIGHT = 600;

class Window
{
public:
  Window();

  Window(uint32_t width, uint32_t height, const char* name = "Window");

private:
  GLFWwindow* pWinGLFW;
};


#endif // !WINDOW_H
