/* 
* Author: Thomas Moreno Cooper 
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
* 
* Class representing a standard deck of cards
*/


#ifndef DECK_H
#define DECK_H 1

#include <cstdint>
#include <array>


constexpr uint32_t CARDS_PER_SUITE = 13;
constexpr uint32_t NUM_CARDS = CARDS_PER_SUITE * 4;

constexpr char* CARDS[CARDS_PER_SUITE] =
{
  "Ace",
  "Two", 
  "Three",
  "Four",
  "Five",
  "Six",
  "Seven",
  "Eight",
  "Nine",
  "Ten",
  "Jack",
  "Queen",
  "King"
};

constexpr char* SUITES[4] =
{
  "Hearts",
  "Diamonds",
  "Clubs",
  "Spades"
};

constexpr uint32_t SCORES[12] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 }; // omit ace, handled separately

class Deck
{
public:
  Deck(bool shuffle = false);

  uint32_t draw();

private:
  std::array<uint32_t, NUM_CARDS> cards;

  size_t topOfDeck;
};

#endif // !DECK_H