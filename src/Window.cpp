/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* Minimal window class
*/

#include <error.h>
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
    ERROR_MSG("Failed to initialise window!", __FILE__, __LINE__)
  }
}

void Window::onResize(GLFWwindow* pWindow, int width, int height)
{
  Window* window = (Window*)glfwGetWindowUserPointer(pWindow);
  if (window)
  {
    // resize window data
    glViewport(0, 0, width, height);
    window->width = (uint32_t)width;
    window->height = (uint32_t)height;
  }
}

Vector2 Window::getWindowSize() const
{
  return { (float)width, (float)height };
}

void Window::swapBuffers()
{
  glfwSwapBuffers(pWinGLFW);
}