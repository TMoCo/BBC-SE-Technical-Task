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
#include <CardRenderer.h>
#include <Deck.h>
#include <Log.h>
#include <Framebuffer.h> 
#include <Window.h>

class Blackjack
{
  friend class UserInterface;

public:
  Blackjack();

  int play(int numPlayers);

private:
  int init();

  void reset(uint32_t& stopMask, uint32_t& turnCount, CardRenderer& renderer);

  void terminate();

  void getWinners();

protected:
  Window window;

  Framebuffer boardFramebuffer;

  Deck deck;

  std::vector<Player> players;

  bool newGame;

};

#endif // !BLACKJACK_H 
