/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Blackjack class definition
*/

#include <Blackjack.h>
#include <UserInterface.h>
#include <iostream>
#include <thomath.h> // personal maths library

Blackjack::Blackjack()
  : deck{ true }
{ }

int Blackjack::init()
{
  if (!glfwInit())
  {
    std::cerr << "Error! Could not initialise GLFW." << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = { DEFAULT_WIDTH, DEFAULT_HEIGHT, "Blackjack" };

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Error! Could not initialise Glad." << std::endl;
    return -1;
  }

  UserInterface::get().init(&window);

  return 0;
}

int Blackjack::play(int numPlayers)
{
  if (numPlayers > MAX_NUM_PLAYERS || numPlayers < MIN_NUM_PLAYERS)
  {
    std::cerr << "Error! Invalid number of players." << std::endl;
    return -1;
  }

  if (init() != 0)
  {
    std::cerr << "Error! Could not initialise the game." << std::endl;
    return -1;
  }

  players.resize((size_t)numPlayers - 1);
  players.push_back({ false });

  // all players start by drawing 1 card
  for (int i = 0; i < players.size(); ++i)
  {
    uint32_t card = deck.draw();
    printCard(card);
    players[i].setCardBit(card);
  }

  size_t currentPlayer = players.size() - 1;

  uint32_t stopMask = 0;
  uint32_t allStopMask = (1 << (numPlayers)) - 1;
  
  glClearColor(0.0f, 0.69921f, 0.23437f, 1.0f);

  while (true)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    UserInterface::get().set(this);

    // process turn only if they can play
    if (!(stopMask & (1 << currentPlayer)))
    {
      if (players[currentPlayer].isAi)
      {
        players[currentPlayer].action = Action::HIT; // ai always draws for now
      }

      std::cout << "\n\nPlayer " << currentPlayer + 1 << "'s turn!\n";
      std::cout << "Action taken: " << players[currentPlayer].action << "\n";
      std::cout << !(stopMask & (1 << currentPlayer)) << "\n";

      // user input here determines action taken
      if (players[currentPlayer].action != Action::NONE)
      {
        if (players[currentPlayer].action == Action::HIT)
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
        else if (players[currentPlayer].action == Action::STAND)
        {
          // don't draw any more cards
          players[currentPlayer].state = PlayerState::STANDING;
          stopMask |= 1 << currentPlayer;
        }

        // reset player action and advance to next player
        players[currentPlayer].action = Action::NONE;
        currentPlayer = ++currentPlayer == players.size() ? 0 : currentPlayer;
      }
    }
    else
    {
      players[currentPlayer].action = Action::NONE;
      currentPlayer = ++currentPlayer == players.size() ? 0 : currentPlayer;
    }

    // all players are either standing or bust
    if (allStopMask == stopMask)
    {
      // reveal dealer's card and decide the winner
      break;
    }

    /*
    if (players[numPlayers - 1].state == PlayerState::BUST)
    {
      // stop game
      std::cout << "You're bust! Too bad." << std::endl;
      break;
    }
    */

    UserInterface::get().draw();

    window.swap();
    glfwPollEvents();
  }

  return 0;
}

void Blackjack::terminate()
{
  UserInterface::get().terminate();

  glfwDestroyWindow(window.pWinGLFW);

  glfwTerminate();
}

void Blackjack::printCard(uint32_t cardId) const
{
  std::cout << CARDS[cardId % CARDS_PER_SUITE] << " of " << SUITES[cardId / CARDS_PER_SUITE] << std::endl;
}
