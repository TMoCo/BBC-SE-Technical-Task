/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Player class definition
*/

#include <Player.h>
#include <Deck.h>

Player::Player(bool isAi)
  : isAi{ isAi }, state{ PlayerState::PLAYING }, hand{}, action{ Action::NONE }
{ }

void Player::setCardBit(uint32_t cardId)
{
  hand[cardId / CARDS_PER_SUITE] |= 1 << (cardId % CARDS_PER_SUITE);
}

uint32_t Player::getScore()
{
  uint32_t score = 0;

  for (uint32_t i = 1; i < CARDS_PER_SUITE; ++i)
  {
    uint32_t card = (1 << i), value = SCORES[i - 1];
    // get number of cards of a given suite and add their value if bit is set
    score += (((hand[0] & card) + (hand[1] & card) + (hand[2] & card) + (hand[3] & card)) >> i) * value;
  }
  
  // handle aces separately
  for (uint32_t i = 0; i < 4; ++i)
  {
    // if room for ace value of 11 use it, otherwise set to 1
    score += (hand[i] & 1) * ((score + 11) < 21 ? 11 : 1);
  }

  return score;
}