/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Deck class definition
*/

#include <error.h>
#include <Deck.h>
#include <random>
#include <iostream>

Deck::Deck()
  : cards{}, topOfDeck{ 0 }
{
  // generate card ids
  for (uint32_t i = 0; i < CARDS_TOTAL; ++i)
  {
    cards[i] = i;
  }
}

uint32_t Deck::draw()
{
  if (topOfDeck == CARDS_TOTAL)
  {
    ERROR_MSG("Ran out of cards to draw.", __FILE__, __LINE__);
    return topOfDeck;
  }

  return cards[topOfDeck++];
}

void Deck::shuffle()
{
  static std::random_device rd;
  static std::mt19937 generator(rd());
  static std::uniform_int_distribution<uint32_t> distribution(0, CARDS_TOTAL - 1);

  // swap cards arbitrarily
  for (int i = 0; i < 10000; ++i)
  {
    std::swap(cards[distribution(generator)], cards[distribution(generator)]);
  }

  topOfDeck = 0; // reset top card position
}