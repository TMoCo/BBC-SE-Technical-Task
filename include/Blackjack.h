/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* The game of Blackjack
*/


#ifndef BLACKJACK_H
#define BLACKJACK_H 1

#include <glad/glad.h>
#include <Player.h>
#include <BoardRenderer.h>
#include <Deck.h>
#include <Log.h>
#include <Framebuffer.h> 
#include <Window.h>

enum GameType : uint32_t
{
  NO_DEALER,
  HOLE_CARD_GAME,
};

class Blackjack
{
  friend class UserInterface;
  friend class BoardRenderer;
  friend class Player;

public:
  Blackjack();

  int play(int numPlayers);

private:
  int init();

  void reset(uint32_t& stopMask, uint32_t& turnCount);

  void terminate();

  void getWinners();

protected:
  Window window;

  Deck deck;

  std::vector<Player> players;

  Player dealer;

  bool newGame, showHands;
};

#endif // !BLACKJACK_H 
