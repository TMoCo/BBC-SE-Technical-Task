/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* Minimal window class
*/

#ifndef WINDOW_H
#define WINDOW_H 1

#include <thomath.h>
#include <cstdint>
#include <GLFW/glfw3.h>

constexpr uint32_t DEFAULT_WIDTH = 1200;
constexpr uint32_t DEFAULT_HEIGHT = 800;

class Window
{
  friend class UserInterface;
  friend class Blackjack;

public:
  Window();

  Window(uint32_t width, uint32_t height, const char* name = "Window", bool makeCurrent = true);
  
  void swapBuffers();

  Vector2 getWindowSize() const;

private:
  static void onResize(GLFWwindow* window, int width, int height);

protected:
  GLFWwindow* pWinGLFW;

  uint32_t width, height;
};


#endif // !WINDOW_H
