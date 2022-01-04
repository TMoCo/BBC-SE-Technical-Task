/*
* Author: Thomas Moreno Cooper
* Date Created: 03/01/2022
* Copyright 2022 under MIT License
*
* Card class declaration, used for rendering 
*/

#ifndef CARD_RENDERER_H
#define CARD_RENDERER_H 1

#include <cstdint>
#include <Shader.h>
#include <Framebuffer.h>
#include <Texture.h>

static const uint32_t QUAD_INDICES[6] =
{
  0, 1, 3,
  1, 2, 3
};

static const float QUAD_VERTICES[24] =
{
  // x      y     z     u     v
   0.5f,  1.0f, 0.0f, 1.0f, 1.0f, // v0
   0.5f, -1.0f, 0.0f, 1.0f, 0.0f, // v1
  -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, // v2
  -0.5f,  1.0f, 0.0f, 0.0f, 1.0f  // v3
};

// Given the card atlas image (1600x714) and a card index, we determine how to scale vertex uv coordinates
// to render the desired card texture
static const Vector2 ATLAS_CARD_COORDINATES[13] =
{
  { 0.0f,         0.25f }, // Ace
  { 5.0f / 14.0f, 0.0f  }, // Two
  { 4.0f / 14.0f, 0.0f  }, // Three
  { 3.0f / 14.0f, 0.0f  }, // Four
  { 2.0f / 14.0f, 0.0f  }, // Five
  { 1.0f / 14.0f, 0.0f  }, // Six
  { 0.0f        , 0.0f  }, // Seven
  { 6.0f / 14.0f, 0.25f }, // Eight
  { 5.0f / 14.0f, 0.25f }, // Nine
  { 4.0f / 14.0f, 0.25f }, // Ten
  { 3.0f / 14.0f, 0.25f }, // Jack
  { 2.0f / 14.0f, 0.25f }, // Queen
  { 1.0f / 14.0f, 0.25f }  // King
};

static const Vector2 ATLAS_SUITE_OFFSETS[4]
{
  { 0.0f, 0.0f }, // hearts
  { 0.5f, 0.5f }, // diamonds
  { 0.0f, 0.5f }, // clubs
  { 0.5f, 0.0f }  // spades
};

class BoardRenderer
{
  friend class Blackjack;
public:
  BoardRenderer();

  ~BoardRenderer();

  void drawCard(uint32_t cardId);

  void drawCardBack();

  void drawBoard(Blackjack* blackjack, bool showHands);

public:
  Framebuffer boardFramebuffer;

protected:
  Shader cardShader;

private:

  Texture cardBack, cardAtlas;

  uint32_t VBO, VAO, EBO;

};


#endif // !CARD_RENDERER_H

