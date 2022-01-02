/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* The game of Blackjack
*/


#ifndef BLACKJACK_H
#define BLACKJACK_H 1

#include <Player.h>
#include <Deck.h>
#include <Log.h>
#include <glad/glad.h> // must be before glfw
#include <Window.h>

class Blackjack
{
  friend class UserInterface;

public:
  Blackjack();

  int play(int numPlayers);

private:
  int init();

  void terminate();

  void printCard(uint32_t cardId) const;

  const char* setCardString(char* string, size_t count, uint32_t carId) const;

protected:
  Window window;

  Deck deck;

  std::vector<Player> players;
};

#endif // !BLACKJACK_H 
