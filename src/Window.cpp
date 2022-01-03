/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* Minimal window class
*/

#include <Window.h>
#include <iostream>

Window::Window()
  : pWinGLFW{ nullptr }, width{ 0 }, height{ 0 }
{ }

Window::Window(uint32_t width, uint32_t height, const char* name, bool makeCurrent)
  : pWinGLFW{ glfwCreateWindow(width, height, name, nullptr, nullptr) }, width{ width }, height{ height }
{
  // init glfw window
  if (pWinGLFW)
  {
    glfwSetFramebufferSizeCallback(pWinGLFW, onResize);
    if (makeCurrent)
    {
      glfwMakeContextCurrent(pWinGLFW);
    }
  }
  else
  {
    std::cerr << "Error initialising window!\n";
  }
}

void Window::onResize(GLFWwindow* pWindow, int width, int height)
{
  Window* window = (Window*)glfwGetWindowUserPointer(pWindow);
  if (window)
  {
    // resize window data
    window->width = width;
    window->height = height;
  }
}

Vector2 Window::getWindowSize()
{
  return { (float)width, (float)height };
}

void Window::swap()
{
  glfwSwapBuffers(pWinGLFW);
}