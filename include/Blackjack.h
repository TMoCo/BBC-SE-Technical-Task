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

enum Action : uint32_t
{
  NONE,
  HIT,
  STAND
};


class Blackjack
{
public:
  Blackjack();

  int play(int numPlayers);

private:
  void printCard(uint32_t cardId) const;

  std::vector<Player> players;

  Deck deck;
};

#endif // !BLACKJACK_H 
