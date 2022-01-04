/*
* Author: Thomas Moreno Cooper
* Date Created: 03/01/2022
* Copyright 2022 under MIT License
*
* Card class definition
*/

#include <glad/glad.h>
#include <Blackjack.h>
#include <BoardRenderer.h>
#include <Window.h>

#ifndef NDEBUG
BoardRenderer::BoardRenderer()
  : VBO{ 0 }, VAO{ 0 }, EBO{ 0 }, cardShader{ "..\\src\\card.vert", "..\\src\\card.frag" },
  cardBack{ "..\\cardback.jpg" }, cardAtlas{ "..\\cardfront.jpg" }, boardFramebuffer{ DEFAULT_WIDTH, DEFAULT_HEIGHT }
#else
BoardRenderer::BoardRenderer()
  : VBO{ 0 }, VAO{ 0 }, EBO{ 0 }, cardShader{ "src\\card.vert", "src\\card.frag" },
  cardBack{ "cardback.jpg" }, cardAtlas{ "cardfront.jpg" }, boardFramebuffer{ DEFAULT_WIDTH, DEFAULT_HEIGHT }
#endif // !NDEBUG
{ 
  cardShader.use(); // only shader used in application, keep always on

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  // generate vertex buffers for drawing quads
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);

  // interleave vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

BoardRenderer::~BoardRenderer()
{
  /*
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  */
  glDeleteVertexArrays(1, &VAO);
}

void BoardRenderer::drawBoard(Blackjack* blackjack, bool showHands)
{
  // refresh board 
  boardFramebuffer.bind();
  glClearColor(0.0f, 0.29921f, 0.13437f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  float cardScale = 0.3f;
  cardShader.setFloat("cardScale", cardScale);
  Vector2 stride = { 1.2f * cardScale, 0.0f }, cardPosition = { -0.8f, -0.65f };
  // render player 1's hand
  for (auto& card : blackjack->players.front().hand)
  {
    cardShader.setVec2("cardPosition", cardPosition);
    drawCard(card);
    cardPosition += stride;
  }

  // render other players' hands
  cardScale = 1.0f / (blackjack->players.size() - 1);
  cardShader.setFloat("cardScale", cardScale);
  if (showHands)
  {
    for (int i = 1; i < blackjack->players.size(); ++i)
    {
      float handScalar = 1.0f / (float)blackjack->players[i].hand.size();
      stride = { cardScale * handScalar * 0.8f, -0.1f * handScalar };
      cardPosition = { -0.9f + cardScale * 0.5f + (i - 1) * 1.8f * cardScale, 0.8f - cardScale * 0.5f };
      for (auto& card : blackjack->players[i].hand)
      {
        cardShader.setVec2("cardPosition", cardPosition);
        drawCard(card);
        cardPosition += stride;
      }
    }
  }
  else
  {
    for (int i = 1; i < blackjack->players.size(); ++i)
    {
      float handScalar = 1.0f / (float)blackjack->players[i].hand.size();
      stride = { cardScale * handScalar * 0.8f, -0.1f * handScalar };
      cardPosition = { -0.9f + cardScale * 0.5f + (i - 1) * 1.8f * cardScale, 0.8f - cardScale * 0.5f };
      for (auto& card : blackjack->players[i].hand)
      {
        cardShader.setVec2("cardPosition", cardPosition);
        blackjack->players[i].state == PlayerState::BUST ? drawCard(card) : drawCardBack();
        cardPosition += stride;
      }
    }
  }
}

void BoardRenderer::drawCardBack()
{
  static Vector2 texCoordScale = { 1.0f, 1.0f };
  cardBack.bind(0); // card back texture
  cardShader.setVec4("texTransform", { 1.0f, 1.0f, 0.0f, 0.0f });
  glDrawElements(GL_TRIANGLES, sizeof(QUAD_INDICES) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}

void BoardRenderer::drawCard(uint32_t cardId)
{
  // given a card scale and a card position, build an affine transform for the card's texture coordinates
  static Vector2 texScale = { 1.0f / 14.0f, 0.25f };
  Vector2 texOffset = ATLAS_CARD_COORDINATES[cardId % 13] + ATLAS_SUITE_OFFSETS[(cardId / 13) & 3];
  cardAtlas.bind(0);
  cardShader.setFloat("cardZ", 1.0f);
  cardShader.setVec4("texTransform", { texScale[0], texScale[1], texOffset[0], texOffset[1] });
  glDrawElements(GL_TRIANGLES, sizeof(QUAD_VERTICES) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}