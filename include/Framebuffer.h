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

  Framebuffer(uint32_t width, uint32_t height);

  ~Framebuffer();

  void bind();

protected:
  uint32_t colourBuffer; // give access to userinterface for drawing

private:
  uint32_t FBO;

  uint32_t width, height;

};

#endif // !FRAMEBUFFER_H