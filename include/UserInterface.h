/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* User interface class
*/

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H 1

class Blackjack;
class Window;

class UserInterface
{
  UserInterface();

public:
  static UserInterface& get();

  void set(Blackjack* blackjack);

  void draw();

  void init(Window* window);

  void terminate();
};

#endif // !USER_INTERFACE_H

