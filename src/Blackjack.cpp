/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Blackjack class definition
*/

#include <error.h>
#include <Blackjack.h>
#include <Texture.h>
#include <UserInterface.h>
#include <iostream>
#include <Shader.h>
#include <thomath.h>

Blackjack::Blackjack()
  :type{ GameType::NO_DEALER }, newGame{ false }, showHands{ false }
{ }

int Blackjack::init()
{
  if (!glfwInit())
  {
    ERROR_MSG("Failed to initialise GLFW.", __FILE__, __LINE__);
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = { DEFAULT_WIDTH, DEFAULT_HEIGHT, "Blackjack" };
  glfwSetWindowUserPointer(window.pWinGLFW, &window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    ERROR_MSG("Failed to initialise Glad.", __FILE__, __LINE__);
    return -1;
  }

  UserInterface::get().init(&window);

  return 0;
}

void Blackjack::reset(uint32_t& stopMask, uint32_t& turnCount)
{
  deck = Deck{ true }; // refresh deck

  Log::get()->clear();
  Log::add("-------------------------------\nNew game started!\nAll players draw two cards.\n");

  dealer.hand.clear();
  dealer.state = PlayerState::PLAYING;

  for (int i = 0; i < players.size(); ++i)
  {
    players[i].state  = PlayerState::PLAYING;
    players[i].action = Action::NONE;
    players[i].hand.clear();
    players[i].hand.push_back(deck.draw());
  }
  dealer.hand.push_back(deck.draw());

  for (int i = 0; i < players.size(); ++i)
  {
    players[i].hand.push_back(deck.draw());
  }
  dealer.hand.push_back(deck.draw());

  stopMask = 0;
  turnCount = 1;

  newGame = false;
}

int Blackjack::play(int numPlayers)
{
  if (numPlayers > MAX_NUM_PLAYERS || numPlayers < MIN_NUM_PLAYERS)
  {
    ERROR_MSG("Invalid number of players.", __FILE__, __LINE__);
    return -1;
  }

  if (init() != 0)
  {
    ERROR_MSG("Failed to initialise the game.", __FILE__, __LINE__);
    return -1;
  }

  // init renderer
  BoardRenderer renderer;

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // init players
  players.resize((size_t)numPlayers);
  players.front().isAi = false;

  // reset game management
  uint32_t stopMask, turnCount, allStopMask = (1 << (numPlayers)) - 1;
  size_t currentPlayer = 0;
  bool newTurn = true;

  reset(stopMask, turnCount);

  renderer.drawBoard(this, showHands);

  while (!glfwWindowShouldClose(window.pWinGLFW))
  {
    UserInterface::get().set(this, &renderer);

    // at least one player is still playing
    if (allStopMask != stopMask)
    {
      if (newTurn)
      {
        newTurn = false;

        Log::add("-------------------------------\nTurn %u\n-------------------------------\n", turnCount++);
        renderer.drawBoard(this, showHands);

        if (!(stopMask & 1)) // if player 1 is still playing, prompt them
        {
          Log::add("Player %u, choose an action.\n", 1);
          Log::add("Your current score is %u.\n", players.front().getScore());
        }
      }

      Player& player = players[currentPlayer];

      // process player's turn only if they can play
      if (!(stopMask & (1 << currentPlayer)))
      {

        if (player.isAi)
        {
          player.determineAction(this);
        }

        // user input here determines action taken
        if (player.action != Action::NONE)
        {
          if (player.action == Action::HIT)
          {
            Log::add("Player %u decided to HIT.\n", currentPlayer + 1);
            
            player.hand.push_back(deck.draw());

            if (player.getScore() > 21)
            {
              player.state = PlayerState::BUST;
              stopMask |= 1 << currentPlayer;
              Log::add("/!\\ Player %u has gone bust!\n", currentPlayer + 1);
            }
          }
          else if (player.action == Action::STAND)
          {
            player.state = PlayerState::STANDING;
            stopMask |= 1 << currentPlayer;
            Log::add("Player %u decided to STAND.\n", currentPlayer + 1);
          }

          // reset player action and advance to next player
          player.action = Action::NONE;
          if (++currentPlayer == players.size())
          {
            newTurn = true;
            currentPlayer = 0;
          }
        }
      }
      else
      {
        player.action = Action::NONE;
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
        renderer.drawBoard(this, true);
      }
      else if (players.front().state == PlayerState::BUST) // Player 1 lost, play again?
      {
        // stop game
        renderer.drawBoard(this, true);
        Log::add("You're bust! Too bad.\nPlay again?\n");
        stopMask = allStopMask;
      }
    }

    if (newGame)
    {
      reset(stopMask, turnCount);
      renderer.drawBoard(this, showHands);
      currentPlayer = 0;
      newTurn = true;
    }

    // bind default framebuffer for drawing gui
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
  if (type == GameType::HOLE_CARD_GAME)
  {
    highestScore = dealer.getScore();
    while (highestScore < 17) // dealer draws till score is greater than or equal to 17
    {
      dealer.hand.push_back(deck.draw());
      highestScore = dealer.getScore();
    }

    if (highestScore > 21)
    {
      dealer.state = PlayerState::BUST;
      Log::add("Dealer has gone bust!\n");
      for (uint32_t i = 0; i < players.size(); ++i)
      {
        if (players[i].state != PlayerState::BUST)
        {
          winnersMask |= 1 << i;
        }
      }
    }
    else
    {
      Log::add("The dealer's score to beat is %u.\n", dealer.getScore());
      if (dealer.hand.size() == 2 && highestScore == 21)
      {
        needBlackjack = true; // the dealer has a natural, another is needed to tie
        Log::add("The dealer got a natural!\n");
      }


      if (needBlackjack)
      {
        for (uint32_t i = 0; i < players.size(); ++i)
        {
          // conditions to tie with dealer
          if (players[i].state != PlayerState::BUST && players[i].getScore() == 21 && players[i].hand.size() == 2)
          {
            winnersMask |= 1 << i;
          }
        }

        // display tied players
        for (uint32_t i = 0; i < players.size(); ++i)
        {
          if (winnersMask & (1 << i))
          {
            Log::add("Player %u tied with the dealer.\n", i + 1);
          }
        }
      }
      else
      {
        for (uint32_t i = 0; i < players.size(); ++i)
        {
          // conditions to beat the dealer
          uint32_t playerScore = players[i].getScore();
          if (players[i].state != PlayerState::BUST && playerScore > highestScore)
          {
            winnersMask |= 1 << i;
          }
          // conditions to tie with the dealer
          else if (players[i].state != PlayerState::BUST && playerScore == highestScore)
          {
            if (highestScore == 21 && players[i].hand.size() == 2) // special case for 21
            {
              Log::add("Player %u got a natural!\n", i + 1); // a natural 21 beats other 21s
              winnersMask |= 1 << i;
            }
            else
            {
              Log::add("Player %u tied with the dealer.\n", i + 1);
            }
          }
        }
      }
    }

    if (winnersMask == 0)
    {
      Log::add("Nobody beat the dealer, better luck next time!\n");
    }
    else
    {
      // display the winner
      for (uint32_t i = 0; i < players.size(); ++i)
      {
        if (winnersMask & (1 << i))
        {
          Log::add("Player %u beat the dealer!\n", i + 1);
        }
      }
    }
  }
  else if (type == GameType::NO_DEALER)
  {
    for (uint32_t i = 0; i < players.size(); ++i)
    {
      if (players[i].state != PlayerState::BUST)
      {
        uint32_t playerScore = players[i].getScore();
        if (playerScore > highestScore)
        {
          highestScore = playerScore;
          winnersMask = 0; // reset mask
          winnersMask |= 1 << i;
        }

        if (playerScore == 21)
        {
          if (players[i].hand.size() == 2 && !needBlackjack)
          {
            winnersMask = 0; // first blackjack detected, invalidated previous non blackjack 21 scores
            needBlackjack = true;
          }

          if (!needBlackjack)
          {
            winnersMask |= 1 << i;
          }
          else if (players[i].hand.size() == 2)
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

    if (_mm_popcnt_u32(winnersMask) > 1) // if we have multiple winners check for ties
    {
      Log::add("It's a tie!\n");
    }

    // display the winner
    for (uint32_t i = 0; i < players.size(); ++i)
    {
      if (winnersMask & (1 << i))
      {
        Log::add("Player %u wins!\n", i + 1);
      }
    }
  }  
}