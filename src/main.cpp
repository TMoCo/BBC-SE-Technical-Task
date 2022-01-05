/*
* Author: Thomas Moreno Cooper
* Date Created: 01/01/2022
* Copyright 2022 under MIT License
*
* Entry point
*/

#include <Blackjack.h>

int main(int argc, char** argv)
{
#ifdef NDEBUG
  if (argc < 2)
  {
    fprintf(stderr, "Invalid number of arguments.\nUsage:\tC:\\Blackjack.exe nPlayers\nnPlayers allowed: from 5 to 9.\n");
    fflush(stderr);
    return -1;
  }

  Blackjack blackjack;  
  return blackjack.play(atoi(argv[1]));
#else
  Blackjack blackjack;
  return blackjack.play(5);
#endif // NDEBUG
}
