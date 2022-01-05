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
{}

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

uint32_t Player::getScoreWithoutAces()
{
  uint32_t aces = 0, score = 0;
  for (uint32_t& card : hand)
  {
    score += CARD_VALUES[card % 13];
    aces -= ((1 << (card % 13)) & 1) * 11; // remove ace 
  }
  return score;
}

Action Player::determineAction(Blackjack* game)
{
  uint32_t score;
  if (game->type == GameType::NO_DEALER)
  {
    score = getScore();
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
  }
  else if (game->type == GameType::HOLE_CARD_GAME)
  {
    if (hasAces())
    {
      score = getScoreNoAces();
      if (score < 7)
      {
        action = Action::HIT;
      }
      else if (score == 8)
      {
        action = BASIC_STRATEGY_SOFT[game->dealer.hand.front() % 13];
      }
      else
      {
        action = Action::STAND;
      }
    }
    else
    {
      score = getScore();
      if (score < 12)
      {
        action = Action::HIT;
      }
      else if (score == 12)
      {
        action = BASIC_STRATEGY[0][game->dealer.hand.front() % 13];
      }
      else if (score < 17)
      {
        action = BASIC_STRATEGY[1][game->dealer.hand.front() % 13];
      }
      else if (score == 17)
      {
        action = BASIC_STRATEGY[2][game->dealer.hand.front() % 13];
      }
      else
      {
        action = Action::STAND;
      }
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

void Player::reset()
{
  hand.clear();
  state = PlayerState::PLAYING;
  action = Action::NONE;
}