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

// strategy based on dealer's face up card
static constexpr Action BASIC_STRATEGY[5][13] =
{
  {   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT }, // score < 12
  {   HIT,   HIT, STAND, STAND, STAND,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT }, // score == 12
  { STAND, STAND, STAND, STAND, STAND,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT }, // 12 < score < 17
  { STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND,   HIT }, // score == 17
  { STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND }  // score > 17
};

class Blackjack;

class Player
{
public:
  Player(bool isAi = true);

  uint32_t getScore();

  Action determineAction(Blackjack* game);

  uint32_t countCards();

  bool hasAces();

public:
  PlayerState state;

  std::vector<uint32_t> hand;

  Action action;
  
  bool isAi;
};

#endif // !PLAYER_H
