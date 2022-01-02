# BBC Software Engineering Graduate Scheme Technical Task

## Overview
The given task is to make a program that plays the game of Blackjack. The basic scenatro is quite straightforward: simulate a deck of 52 cards and draw cards to get as close to 21 as possible.

The game functions by letting players perform different actions, like hitting or standing, which may change their state. A player is either in a playing, busted or standing state. Busted and standing players can no longer perform any actions and are waiting for every other player to be either busted or standing. When this condition is met, the winner or winners are determined following Blackjack rules: The player with the highest score wins. Players with the same high score are tied. The exception to this rule is when the highest score is 21 in which case players with a "natural blackjack" (21 in their starting hand) take precedence over players with the same score but with more than two cards.

## Design Decisions
I have chosen C++ as this is the language I am the most comfortable in and allows me to add graphics to the application.

Cards are uniquely identified by an unsigned integer. Using the modulus operator and integer division, it's suite and rank can easily be retrieved. For instance with the set of suites { hearts, diamonds, clubs, spades }, the card with id 32 is a 6 of clubs (32 % 13 = 6 and 32 / 13 = 2 where the third suite is clubs). This allows for flexibility in the number of decks the game can use (Blackjack is commonly played with 1 to 8 decks). A deck of cards is modeled by an array of card indices and is shuffled by repeatedly swapping two cards at random positions in the deck. A player's hand consists of an array of 16 bit unsigned integers, one for each suite used in the deck. Bits 1 to 12 represent the ranks 2 to King with bit 0 reprsenting the Ace. Counting cards is achieved by looping over the hand and counting bits set to 1. Counting cards of a certain rank can be achieved by looping over each hand and adding the bitwise and operation of the rank's bit with the suite:

```cplusplus
 uint32_t Player::countCardRankBits(uint32_t cardRank)
{
  uint32_t count = 0, rankBit = 1 << cardRank;
  for (uint32_t i = 0; i < CARDS_TOTAL / CARD_RANKS; ++i)
  {
    count += (hand[i] & rankBit);
  }
  return count >> cardRank;
}
 ```
This avoids unnescesary branching when looking for a specific card rank, namely for aces:
```cplusplus
  uint32_t aces = countCardRankBits(0) * 11;
  while (score + aces > 21 && aces > 4) // stop if we have a valid score or aces already have value 1 (busted!)
  {
    aces -= 10;
  }
```
The game is run in a continuous loop which is tied to the lifetime of the window. In this loop the GUI is set and rendered along with the players' hands of cards in future versions.

The initial solution consist of a simple GUI with options to either Hit or Stand based on the player's current hand. This solution will also feature basic AI oponents that have the same set of options available. Players will be able to start new games by interacting with a simple menu bar.

Version 2 will feature a rendering of player hands on the right hand side of the GUI.

Version 3 will feature hand splitting as an option.

## Screnshots of development:
Screenshot of application V1, featuring a text log displaying game events and scores as players make their decisions. Simulated oponents use the basic strategy for the number of decks used in the game. 
![image](https://user-images.githubusercontent.com/56483943/147889522-3f954cf2-a7af-4ca9-a921-60faa2f6b5a2.png)
