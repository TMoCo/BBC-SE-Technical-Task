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

uint32_t Player::getScore()
{
  uint32_t aces = 0, score = 0;
  for (uint32_t& card : hand)
  {
    score += CARD_VALUES[card % 13];
    aces += (1 << (card % 13)) & 1;
  }

  while (score > 21 && aces > 0) 
  {
    score -= 10; // remove aces
    aces--;
  }

  return score;
}

uint32_t Player::countCards()
{
  uint32_t count = 0; // accumulate the total bits set in hand
  for (uint32_t i = 0; i < CARDS_TOTAL / CARD_RANKS; ++i)
  {
    uint16_t suite = hand[i];
    for (; suite; count++)
    {
      suite &= suite - 1; // clear the least significant bit set
    }
  }
  return count;
}

Action Player::determineAction(Blackjack* game)
{
  // determine if player has an ace
  uint32_t score = getScore();
  if (hasAces())
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

bool Player::hasAces()
{
  bool hasAces = false;
  for (auto& card : hand)
  {
    hasAces |= (1 << (card % 13)) & 1; // 0 = false, !0 = true
  }
  return hasAces;
}
