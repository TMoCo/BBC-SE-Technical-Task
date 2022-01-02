/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Player class definition
*/

#include <Player.h>
#include <Blackjack.h>
#include <Deck.h>
#include <iostream>

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
    uint32_t card = 1 << i, value = SCORES[i - 1];
    // get number of cards of a given suite and add their value if bit is set
    score += ((hand[0] & card) + (hand[1] & card) + (hand[2] & card) + (hand[3] & card)) * value >> i;
  }
  
  // handle aces separately
  uint32_t aces = ((hand[0] & 1) + (hand[1] & 1) + (hand[2] & 1) + (hand[3] & 1)) * 11;
  while (score + aces > 21 && aces > 4) // stop if aces can't be minimised or we have a valid score
  {
    aces -= 10;
  }

  return score + aces;
}

Action Player::determineAction(Blackjack* game)
{
  // based on player's score, determine action to take
  uint32_t score = getScore();

  // determine if player has an ace
  if ((hand[0] & 1) | (hand[1] & 1) | (hand[2] & 1) | (hand[3] & 1))
  {
    if (score < 13)
    {
      action = Action::HIT;
    }
    else
    {
      action = Action::STAND;
    }
  }
  else
  {
    if (score < 18)
    {
      action = Action::HIT;
    }
    else
    {
      action = Action::STAND;
    }
  }
  return action;
}