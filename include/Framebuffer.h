/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* The simplest possible framebuffer with a single colour buffer
*/

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H 1

#include <cstdint>

class Framebuffer
{
  friend class UserInterface;
public:
  Framebuffer();

  ~Framebuffer();

  void build(uint32_t width, uint32_t height);

  void bind();

protected:
  uint32_t colourBuffer; // give access to userinterface for drawing

private:
  void destroy();

  uint32_t FBO;
};

#endif // !FRAMEBUFFER_H