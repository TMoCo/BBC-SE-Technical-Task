/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Deck class definition
*/

#include <Deck.h>
#include <random>
#include <iostream>

Deck::Deck(bool shuffle)
  : cards{}, topOfDeck{ 0 }
{
  // populate cards
  for (uint32_t i = 0; i < NUM_CARDS; ++i)
  {
    cards[i] = i;
  }

  if (shuffle)
  {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<uint32_t> distribution(0, NUM_CARDS - 1);

    // swap cards arbitrarily
    for (int i = 0; i < 10000; ++i)
    {
      std::swap(cards[distribution(generator)], cards[distribution(generator)]);
    }
  }
}

uint32_t Deck::draw()
{
  if (topOfDeck == NUM_CARDS)
  {
    std::cerr << "Error! No cards remaining to draw." << std::endl;
    return (uint32_t) topOfDeck;
  }

  return cards[topOfDeck++];
}