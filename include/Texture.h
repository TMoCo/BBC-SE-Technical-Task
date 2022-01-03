/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Texture class, interfaces between OpenGL and disk image data
*/

#ifndef TEXTURE_H
#define TEXTURE_H 1

#include <thomath.h>
#include <glad/glad.h>

class Texture
{
  friend class TextureLoader;
  friend class UserInterface;
public:
  Texture();

  Texture(const char* path);

  ~Texture();

  void bind(int unit = -1);

//protected:
  GLenum format;

  uint32_t texId;
};

#endif // !TEXTURE_H
