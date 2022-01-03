/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Card class definition
*/

#include <CardRenderer.h>
#include <glad/glad.h>

CardRenderer::CardRenderer()
  : VBO{ 0 }, VAO{ 0 }, EBO{ 0 }, cardShader{ "..\\src\\card.vert", "..\\src\\card.frag" }, 
  cardBack{ "..\\cardback.jpg" }, cardAtlas{ "..\\cardfront.jpg" }
{ 
  cardShader.use(); // only shader used in application, keep always on

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  // generate vertex buffers for drawing quads
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // interleave vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

CardRenderer::~CardRenderer()
{
  /*
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  */
  glDeleteVertexArrays(1, &VAO);
}

void CardRenderer::drawCardBack()
{
  // set card back texture and card position on board
  cardBack.bind(0);
  glDrawElements(GL_TRIANGLES, sizeof(quadIndices) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}

void CardRenderer::drawCard(uint32_t cardRank, uint32_t cardSuite)
{

}