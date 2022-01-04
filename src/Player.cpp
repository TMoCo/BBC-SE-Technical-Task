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
  hand[cardId / CARD_RANKS] |= 1 << (cardId % CARD_RANKS);
}

uint32_t Player::getScore()
{
  uint32_t aces = 0, score = 0;
  for (auto& card : hand)
  {
    score += CARD_VALUES[card % 13];
    aces += (1 << (card % 13)) & 1;
  }
  aces *= 11;
  while (score + aces > 21 && aces > 4) 
  {
    aces -= 10;
  }

  return score + aces;
}

uint32_t Player::countCardRankBits(uint32_t cardRank)
{
  uint32_t count = 0, rankBit = 1 << cardRank;
  for (uint32_t i = 0; i < CARDS_TOTAL / CARD_RANKS; ++i)
  {
    count += (hand[i] & rankBit);
  }
  return count >> cardRank;
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
