/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Card class declaration, used for rendering 
*/

#ifndef CARD_H
#define CARD_H 1

#include <cstdint>
#include <Texture.h>

static constexpr const uint32_t quadIndices[6] =
{
  0, 1, 3,
  1, 2, 3
};

static constexpr const float quadVertices[24] =
{
  // x      y     z     u     v
   0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // v0
   0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // v1
  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // v2
  -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // v3
};

class Card
{

public:
  Card();

  ~Card();

  void draw();

  uint32_t VBO, VAO, EBO;

private:

};


#endif // !CARD_H

