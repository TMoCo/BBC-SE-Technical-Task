/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Framebuffer class definition
*/

#include <glad/glad.h>
#include <Framebuffer.h>
#include <iostream>

Framebuffer::Framebuffer()
  : FBO{ 0 }, colourBuffer{ 0 }
{ }

Framebuffer::~Framebuffer()
{
  destroy();
}

void Framebuffer::build(uint32_t width, uint32_t height)
{
  destroy(); 
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  // colour buffer
  glGenTextures(1, &colourBuffer);
  glBindTexture(GL_TEXTURE_2D, colourBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourBuffer, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cerr << "Error! Framebuffer is not complete." << std::endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::destroy()
{
  if (colourBuffer)
  {
    glDeleteTextures(1, &colourBuffer);
  }

  if (FBO)
  {
    glDeleteFramebuffers(1, &FBO);
  }
}

void Framebuffer::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}