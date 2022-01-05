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
static constexpr Action BASIC_STRATEGY[3][13] =
{
//    ace,   two, three,  four,  five,   six, seven, eight,  nine,   ten,  jack, queen,  king    <- dealer front card 
//                                                                                                  score < 12 = HIT
  {   HIT,   HIT,   HIT, STAND, STAND, STAND,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT }, // score == 12
  {   HIT, STAND, STAND, STAND, STAND, STAND,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT,   HIT }, // 12 < score < 17
  {   HIT, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND, STAND }, // score == 17
//                                                                                                  score > 17 = STAND
};

static constexpr Action BASIC_STRATEGY_SOFT[13] = 
{ HIT, STAND, STAND, STAND, STAND, STAND, STAND, STAND, HIT, HIT, HIT, HIT, HIT };

class Blackjack;

class Player
{
public:
  Player(bool isAi = true);

  uint32_t getScore();

  uint32_t getScoreWithoutAces();

  Action determineAction(Blackjack* game);

  bool hasAces();

  void reset();

public:
  PlayerState state;

  std::vector<uint32_t> hand;

  Action action;
  
  bool isAi;
};

#endif // !PLAYER_H
