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

constexpr uint32_t NUM_DECKS   = 4;
constexpr uint32_t CARD_RANKS  = 13;
constexpr uint32_t CARD_SUITS  = 4;
constexpr uint32_t CARDS_TOTAL = CARD_RANKS * CARD_SUITS * NUM_DECKS;

constexpr char* CARD_STRINGS[CARD_RANKS] =
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

constexpr char* SUITE_STRINGS[CARD_SUITS] =
{
  "Hearts",
  "Diamonds",
  "Clubs",
  "Spades"
};

constexpr uint32_t CARD_VALUES[CARD_RANKS] = { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 }; // omit ace, handled separately

class Deck
{
public:
  Deck(bool shuffle = false);

  uint32_t draw();

private:
  std::array<uint32_t, CARDS_TOTAL> cards;

  uint32_t topOfDeck;
};

#endif // !DECK_H