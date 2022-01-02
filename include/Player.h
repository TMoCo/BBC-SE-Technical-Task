/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Class representing a Blackjack player
*/


#ifndef PLAYER_H
#define PLAYER_H 1

#include <cstdint>
#include <vector>

constexpr int MAX_NUM_PLAYERS = 9;
constexpr int MIN_NUM_PLAYERS = 5;

enum PlayerState : uint32_t
{
  STANDING,
  BUST,
  PLAYING,
  CHOOSING
};

enum Action : uint32_t
{
  NONE,
  HIT,
  STAND
};

class Player
{
public:
  Player(bool isAi = true);

  void setCardBit(uint32_t cardId);

  uint32_t getScore();

public:
  PlayerState state;

  uint32_t hand[4];

  Action action;
  
  const bool isAi;
};

#endif // !PLAYER_H
