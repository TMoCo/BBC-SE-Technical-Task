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
  for (int i = 0; i < players.size(); ++i)
  {
    memset(players[i].hand, 0, 4 * sizeof(uint16_t)); // reset hand
    
    // draw two cards
    uint32_t card = deck.draw();
    Log::add("Player %u drew %s of %s\n", i + 1, CARD_STRINGS[card % CARD_RANKS], SUITE_STRINGS[card / CARD_RANKS]);
    players[i].setCardBit(card);

    card = deck.draw();
    Log::add("Player %u drew %s of %s\n", i + 1, CARD_STRINGS[card % CARD_RANKS], SUITE_STRINGS[card / CARD_RANKS]);
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

  players.resize((size_t)numPlayers);
  players.front().isAi = false;

  uint32_t stopMask, turnCount;
  reset(stopMask, turnCount);

  // turn management
  uint32_t currentPlayer = 0, allStopMask = (1 << (numPlayers)) - 1;
  bool newTurn = true;


  glClearColor(0.0f, 0.69921f, 0.23437f, 1.0f);
  while (!glfwWindowShouldClose(window.pWinGLFW))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    UserInterface::get().set(this);

    // at least one player is still playing
    if (allStopMask != stopMask)
    {
      if (newTurn)
      {
        Log::add("-------------------------------\nTurn %u\n-------------------------------\n", turnCount++);
        newTurn = false;
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
            Log::add("Player %u decided to HIT.\n", currentPlayer + 1);
            // draw a card
            uint32_t card = deck.draw();
            Log::add("Player %u drew %s of %s\n", 
              currentPlayer + 1, CARD_STRINGS[card % CARD_RANKS], SUITE_STRINGS[card / CARD_RANKS]);

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
            Log::add("Player %u decided to STAND.\n", currentPlayer + 1);
            // don't draw any more cards
            players[currentPlayer].state = PlayerState::STANDING;
            stopMask |= 1 << currentPlayer;
          }

          // reset player action and advance to next player
          players[currentPlayer].action = Action::NONE;

          if (++currentPlayer == players.size())
          {
            newTurn = true;
            currentPlayer = 0;
          }
        }
      }
      else
      {
        players[currentPlayer].action = Action::NONE;

        if (++currentPlayer == players.size())
        {
          newTurn = true;
          currentPlayer = 0;
        }
      }

      // if we stop playing now, determine the winner 
      if (stopMask == allStopMask)
      {
        Log::add("Finding winner...\n");
        getWinners();
      }
      // lose condition
      else if (players.front().state == PlayerState::BUST)
      {
        // stop game
        Log::add("You're bust! Too bad.\nPlay again?\n");
        stopMask = allStopMask;
      }
    }

    if (newGame)
    {
      reset(stopMask, turnCount);
      currentPlayer = 0;
      newTurn = true;
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

void Blackjack::getWinners()
{
  uint32_t highestScore = 0, winnersMask = 0;
  bool needBlackjack = false;
  
  for (uint32_t i = 0; i < players.size(); ++i)
  {
    if (players[i].state != PlayerState::BUST)
    {
      uint32_t playerScore = players[i].score;
      if (playerScore > highestScore)
      {
        highestScore = playerScore;
        winnersMask = 0; // reset mask
        winnersMask |= 1 << i;
      }

      if (playerScore == 21)
      {
        uint32_t cardCount = players[i].countCards();
        if (cardCount == 2 && !needBlackjack)
        {
          winnersMask = 0; // first blackjack detected, invalidated previous non blackjack 21 scores
          needBlackjack = true;
        }

        if (!needBlackjack)
        {
          winnersMask |= 1 << i;
        }
        else if (cardCount == 2)
        {
          Log::add("Blackjack for player %u!\n", i + 1);
          winnersMask |= 1 << i;
        }
      }
      else if (playerScore == highestScore) 
      {
        winnersMask |= 1 << i;
      }
    }
  }

  Log::add("The score to beat was %u.\n", highestScore);

  // if we have multiple winners check for ties
  if (_mm_popcnt_u32(winnersMask) > 1)
  {
    Log::add("It's a tie!\n");
  }

  for (uint32_t i = 0; i < players.size(); ++i)
  {
    if (winnersMask & (1 << i))
    {
      Log::add("Congratulations player %u, you win!\n", i + 1);
    }
  }
}