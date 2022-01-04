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
  : newGame{ false }, showHands{ false }
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

  Vector2 stride = Vector2{ 0.9f, 0.0f } / (float)(players.size() - 1);
  // now draw remaining players
  for (int i = 0; i < players.size(); ++i)
  {
    Player& player = players[i];
    player.hand.clear();

    // draw two cards
    uint32_t card = deck.draw();
    players[i].hand.push_back(card);
    //Log::add("Player %u drew %s of %s\n", i + 1, CARD_STRINGS[card % CARD_RANKS], SUITE_STRINGS[card / CARD_RANKS]);
    card = deck.draw();
    players[i].hand.push_back(card);
    //Log::add("Player %u drew %s of %s\n", i + 1, CARD_STRINGS[card % CARD_RANKS], SUITE_STRINGS[card / CARD_RANKS]);
    
    //Log::add("Updated score is %u\n", players[i].getScore());
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
  uint32_t stopMask, turnCount;
  reset(stopMask, turnCount);
  uint32_t currentPlayer = 0, allStopMask = (1 << (numPlayers)) - 1;
  bool newTurn = true;

  // initialise the board
  renderer.drawBoard(this, showHands);

  while (!glfwWindowShouldClose(window.pWinGLFW))
  {
    UserInterface::get().set(this, &renderer);

    // at least one player is still playing
    if (allStopMask != stopMask)
    {
      if (newTurn)
      {
        Log::add("-------------------------------\nTurn %u\n-------------------------------\n", turnCount++);
        renderer.drawBoard(this, showHands);

        newTurn = false;
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
          player.determineAction(this); // ai always draws for now
        }

        // user input here determines action taken
        if (player.action != Action::NONE)
        {
          if (player.action == Action::HIT)
          {
            Log::add("Player %u decided to HIT.\n", currentPlayer + 1);
            
            // draw a card
            uint32_t card = deck.draw();
            player.hand.push_back(card);

            //Log::add("Updated score is %u\n", score);

            // check if bust
            if (player.getScore() > 21)
            {
              player.state = PlayerState::BUST;
              stopMask |= 1 << currentPlayer;
              Log::add("/!\\ Player %u has gone bust!\n", currentPlayer + 1);
            }
          }
          else if (player.action == Action::STAND)
          {
            Log::add("Player %u decided to STAND.\n", currentPlayer + 1);
            // don't draw any more cards
            player.state = PlayerState::STANDING;
            stopMask |= 1 << currentPlayer;
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
        renderer.drawBoard(this, true);
        getWinners();
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
        size_t cardCount = players[i].hand.size();
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
      Log::add("Congratulations player %u!\n", i + 1);
    }
  }
}