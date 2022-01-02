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
  : newGame{ false }
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

void Blackjack::reset(uint32_t& stopMask, uint32_t& turnCount)
{
  deck = Deck{ true };

  Log::get()->clear();

  Log::add("-------------------------------\nNew game started! \
All players draw a card.\n-------------------------------\n");

  // all players start by drawing 1 card
  char cardString[25] = {};
  for (int i = 0; i < players.size(); ++i)
  {
    memset(players[i].hand, 0, 4 * sizeof(uint16_t)); // reset hand
    
    // draw two cards
    uint32_t card = deck.draw();
    setCardString(cardString, 25, card);
    Log::add("Player %u drew %s\n", i + 1, cardString);
    players[i].setCardBit(card);
    card = deck.draw();
    setCardString(cardString, 25, card);
    Log::add("Player %u drew %s\n", i + 1, cardString);
    players[i].setCardBit(card);

    Log::add("Updated score is %u\n", players[i].getScore());
    players[i].state = PlayerState::PLAYING;
  }

  stopMask = 0;
  turnCount = 1;

  newGame = false;
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

  uint32_t stopMask, turnCount;
  reset(stopMask, turnCount);

  size_t currentPlayer = 0;
  uint32_t allStopMask = (1 << (numPlayers)) - 1;
  
  glClearColor(0.0f, 0.69921f, 0.23437f, 1.0f);

  while (!glfwWindowShouldClose(window.pWinGLFW))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    UserInterface::get().set(this);

    // at least one player is still playing
    if (allStopMask != stopMask)
    {
      if (currentPlayer == 0)
      {
        Log::add("-------------------------------\nTurn %u\n-------------------------------\n", turnCount++);
      }

      // process player's turn only if they can play
      if (!(stopMask & (1 << currentPlayer)))
      {
        if (players[currentPlayer].isAi)
        {
          players[currentPlayer].determineAction(this); // ai always draws for now
        }

        // user input here determines action taken
        if (players[currentPlayer].action != Action::NONE)
        {
          if (players[currentPlayer].action == Action::HIT)
          {
            // draw a card
            uint32_t card = deck.draw();
            char cardString[25] = {};
            setCardString(cardString, 25, card);
            Log::add("Player %u drew %s\n", currentPlayer + 1, cardString);

            players[currentPlayer].setCardBit(card);

            uint32_t score = players[currentPlayer].getScore();

            Log::add("Updated score is %u\n", score);

            // check if bust
            if (score > 21)
            {
              Log::add("/!\\ Player %u has gone bust!\n", currentPlayer + 1);
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


      // if we stop playing now, determine the winner 
      if (stopMask == allStopMask)
      {
        Log::add("Finding winner...\n");
      }
      // lose condition
      else if (players[numPlayers - 1].state == PlayerState::BUST)
      {
        // stop game
        Log::add("You're bust! Too bad.\nPlay again?\n");
        stopMask = allStopMask;
      }
    }

    if (newGame)
    {
      reset(stopMask, turnCount);
    }

    UserInterface::get().draw();

    window.swap();
    glfwPollEvents();
  }

  Log::writeLog();

  terminate();

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

const char* Blackjack::setCardString(char* string, size_t count, uint32_t cardId) const
{
  sprintf_s(string, count, "%s of %s", CARDS[cardId % CARDS_PER_SUITE], SUITES[cardId / CARDS_PER_SUITE]);
  return string;
}