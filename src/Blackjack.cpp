/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Blackjack class definition
*/

#include <Blackjack.h>
#include <iostream>

Blackjack::Blackjack()
  : deck{ true }
{ }

int Blackjack::play(int numPlayers)
{
  if (numPlayers > MAX_NUM_PLAYERS || numPlayers < MIN_NUM_PLAYERS)
  {
    std::cerr << "Error! Invalid number of players." << std::endl;
    return -1;
  }

  players.resize((size_t)numPlayers - 1);
  players.push_back({ false });

  // all players draw 1 card
  for (int i = 0; i < players.size(); ++i)
  {
    uint32_t card = deck.draw();
    printCard(card);
    players[i].setCardBit(card);
  }

  size_t currentPlayer = players.size() - 1;

  uint32_t stopMask = 0;
  uint32_t allStopMask = (1 << (numPlayers)) - 1;
  
  Action action = Action::NONE;

  while (true)
  {

    // process turn only if they can play
    if (!(stopMask & (1 << currentPlayer)))
    {
      if (players[currentPlayer].isAi)
      {
        action = Action::HIT; // ai always draws for now
      }
      else
      {
        // get from user, actions depend on state
        std::cout << "Your turn! What is your move (1 = hit, 2 = stand): ";
        int a;
        std::cin >> a;
        action = (Action)a;
      }

      std::cout << "\n\nPlayer " << currentPlayer + 1 << "'s turn!\n";
      std::cout << "Action taken: " << action << "\n";
      std::cout << !(stopMask & (1 << currentPlayer)) << "\n";

      // user input here determines action taken
      if (action != Action::NONE)
      {
        if (action == Action::HIT)
        {
          // draw a card
          uint32_t card = deck.draw();
          std::cout << " drew ";
          printCard(card);

          players[currentPlayer].setCardBit(card);

          uint32_t score = players[currentPlayer].getScore();

          std::cout << "Score is " << score << std::endl;

          // check if bust
          if (score > 21)
          {
            std::cout << "Player " << currentPlayer + 1 << " has gone bust!" << std::endl;
            players[currentPlayer].state = PlayerState::BUST;
            stopMask |= 1 << currentPlayer;
          }
        }
        else if (action == Action::STAND)
        {
          // don't draw any more cards
          players[currentPlayer].state = PlayerState::STANDING;
          stopMask |= 1 << currentPlayer;
        }

        // next player
        currentPlayer = ++currentPlayer == players.size() ? 0 : currentPlayer;
      }
    }
    else
    {
      currentPlayer = ++currentPlayer == players.size() ? 0 : currentPlayer;
    }

    // all players are either standing or bust
    if (allStopMask == stopMask)
    {
      // reveal dealer's card and decide the winner
      break;
    }

    action = Action::NONE; // reset action

    /*
    if (players[numPlayers - 1].state == PlayerState::BUST)
    {
      // stop game
      std::cout << "You're bust! Too bad." << std::endl;
      break;
    }
    */
  }

  return 0;
}

void Blackjack::printCard(uint32_t cardId) const
{
  std::cout << CARDS[cardId % CARDS_PER_SUITE] << " of " << SUITES[cardId / CARDS_PER_SUITE] << std::endl;
}
