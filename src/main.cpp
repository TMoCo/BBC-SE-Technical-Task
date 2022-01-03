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
    std::cerr << "Error! Invalid number of arguments.\nUsage:\tC:\\Blackjack.exe nPlayers\nPlayers allowed: from 5 to 9." << std::endl;
    return -1;
  }

  Blackjack blackjack;  

  return blackjack.play(atoi(argv[1]));
#else
  Blackjack blackjack;

  return blackjack.play(6);
#endif // NDEBUG
}
