/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Card class declaration, used for rendering 
*/

#ifndef CARD_RENDERER_H
#define CARD_RENDERER_H 1

#include <cstdint>
#include <Shader.h>
#include <Texture.h>

static constexpr const uint32_t quadIndices[6] =
{
  0, 1, 3,
  1, 2, 3
};

static constexpr const float quadVertices[24] =
{
  // x      y     z     u     v
   0.5f,  1.0f, 0.0f, 1.0f, 1.0f, // v0
   0.5f, -1.0f, 0.0f, 1.0f, 0.0f, // v1
  -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, // v2
  -0.5f,  1.0f, 0.0f, 0.0f, 1.0f  // v3
};

class CardRenderer
{
  friend class Blackjack;
public:
  CardRenderer();

  ~CardRenderer();

  void drawCard(uint32_t cardRank, uint32_t cardSuite);

  void drawCardBack();

protected:
  Shader cardShader;

private:
  Texture cardBack, cardAtlas;

  uint32_t VBO, VAO, EBO;

};


#endif // !CARD_RENDERER_H

