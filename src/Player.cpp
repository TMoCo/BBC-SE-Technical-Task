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
  : isAi{ isAi }, state{ PlayerState::PLAYING }, hand{}, action{ Action::NONE }, score{ 0 }
{ }

void Player::setCardBit(uint32_t cardId)
{
  hand[cardId / CARD_RANKS] |= 1 << (cardId % CARD_RANKS);
}

uint32_t Player::getScore()
{
  score = 0;

  // count times the bit is set for each suite
  for (uint32_t i = 1; i < CARD_RANKS; ++i)
  {
    score += countCardRankBits(i) * CARD_VALUES[i - 1];
  }
  
  // handle aces separately
  uint32_t aces = countCardRankBits(0) * 11;
  while (score + aces > 21 && aces > 4) // stop if aces can't be minimised or we have a valid score
  {
    aces -= 10;
  }

  score += aces;

  return score;
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
  if (countCardRankBits(1))
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

void Player::drawHand()
{

}