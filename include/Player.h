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

#include <Deck.h>

constexpr int MAX_NUM_PLAYERS = 9;
constexpr int MIN_NUM_PLAYERS = 5;

enum PlayerState : uint32_t
{
  PLAYING = 0,
  STANDING = 1,
  BUST = 2
};

enum Action : uint32_t
{
  NONE,
  HIT,
  STAND
};

class Blackjack;

class Player
{
public:
  Player(bool isAi = true);

  void setCardBit(uint32_t cardId);

  uint32_t getScore();

  Action determineAction(Blackjack* game);

  uint32_t countCardRankBits(uint32_t rank);

  uint32_t countCards();

public:
  PlayerState state;

  uint16_t hand[CARDS_TOTAL / CARD_RANKS];

  uint32_t score;

  Action action;
  
  bool isAi;
};

#endif // !PLAYER_H
