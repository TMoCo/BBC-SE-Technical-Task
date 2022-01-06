# BBC Software Engineering Graduate Scheme Technical Task

## Overview
The given task is to make a program that plays the game of Blackjack.

The game functions by letting players perform different actions, like hitting or standing, which may change their state. A player is either in a playing, bust or standing state. Bust and standing players can no longer perform any actions and are waiting for every other player to be either bust or standing. When this condition is met, the winner or winners are determined following Blackjack rules: The player with the highest score wins. Players with the same high score are tied. The exception to this rule is when the highest score is 21 in which case players with a "natural blackjack" (21 in their starting hand) take precedence over players with the same score but with more than two cards.

In Casinos, players bet on whether their hand will beat the dealer's and often have more than one deck of cards used (up to 8). The dealer plays after all other players are either standing or bust by repeatedly hitting until their hand is worth at least 17.

To play the game, download the executable. Make sure to place it in the same directory as the two textures _(cardback.jpg, cardfront.jpg)_ and two shaders _(card.vert, card.frag)_. Run it on the command line with an integer argument for the number of players (5 to 9).
```
C:\Blackjack.exe 5
```
## Design and implementation
I have chosen to implement this application in C++ as this is the language I am the most comfortable with and allows me to add some cute graphics to the application. The first version of the application satisfies the list of scenarios provided in the candidate instructions, as well as providing a GUI for inputting actions and viewing game events in a log.

### Version 1
The first insight is to recognise that cards can be uniquely identified by an unsigned integer id. Using the modulus operator and integer division, it's suite and rank can easily be retrieved. For instance with the set of suites { hearts, diamonds, clubs, spades }, the card with id 32 is a 6 of clubs (32 % 13 = 6 and 32 / 13 = 2 where the third suite is clubs). This allows for flexibility in the number of decks the game can use (Blackjack is commonly played with 1 to 8 decks).

A deck of cards is modeled by an array of unique card ids ranging from 0 to N x 52, where N is the number of decks used, in the `Deck` datastructure. The deck is shuffled by repeatedly swapping two cards at random positions in the array an arbitrary number of times. A counter initialised at 0 whenever the deck is shuffled is used to keep track of the card currently at the top of the deck and is incremented by 1 whenever a card is drawn from a `Deck`:
```cpp
return cards[topOfDeck++]; // return card at the top of the deck
```
A player's hand consists of an STL vector of unsigned integers ```uint32_t```. When a card is drawn, the id at the top of the deck is added to the hand with the push_back operation, increasing the player's hand size.

Computing player scores is achieved by looping over the ids in the players's hand and deducing it's rank. This is used as an index into a 13 element array of score for each rank that are added to the player's overall score. Aces need to be handled a little differently, as they can represent a value of 11 or 1 depending on the hand of the player. The change in the ace's value is the reason why the scored is not stored, as it has to be recomputed anyway.

The algorithm initially adds aces to the score as worth 11 whilst also keeping count of them. To count them without branching, 1 bit is shifted to the left by the card's rank to get a new power of two (for instance, a rank of 16 % 13 = 3 is represented as 1 << 3 = 2^3 or 1000 in binary). This is then bit-anded with the ace's binary encoding of 1 << 0 or simply 1, the result of which will be 1 if the card in question is an ace and 0 otherwise, and is added to the aces tally. 

If the score is greater than 21 and there are aces in the hand, we can subtract 10 to the score to turn them into 1 valued aces.
```cpp
uint32_t Player::getScore()
{
  uint32_t aces = 0, score = 0;
  for (uint32_t& card : hand)
  {
    score += CARD_VALUES[card % 13];
    aces += (1 << (card % 13)) & 1;
  }
  while (score > 21 && aces > 0) 
  {
    score -= 10; // reduce ace
    aces--;
  }
  return score;
 ``` 
Players can either Hit or Stand which will affect their hands, scores and states. Both actions and states are represented as enums in ```Player.h```.
```cpp
enum PlayerState : uint32_t
{
  PLAYING,
  STANDING,
  BUST
};

enum Action : uint32_t
{
  NONE,
  HIT,
  STAND
};
```
To allow for user interaction, the game is run as part of a loop that only ends when the window should close. To process a turn, the application needs to keep track of players still playing and the current player being processed. Players that have gone bust or are standing are represented as bits in a mask, the ```stopMask```, that is checked on their turn. If their bit is set, then their action does not need to be processed and we can move on to the next player. This mask is also checked at then end of the loop to see if the game has ended as all players have stopped playing, (ie: ```stopMask == (1 << NPlayer) - 1)```.

Simulated players will decide on an action as soon as their turn starts but the application must await input from the user on player 1's turn. The ```Action::NONE``` is introduced to let the loop flow freely while player 1 decides their next step by simply not moving on to the next player while the action is still ```Action::NONE```. The HIT and STAND buttons in the GUI change player 1's action, which lets the game proceed to the other players.

Here is the code for processing a player's turn in ```Blackjack::play()```:
```cpp
if (!(stopMask & (1 << currentPlayer)))
{
  if (player.isAi)
  {
    player.determineAction(this); // determine action from game state
  }

  if (player.action != Action::NONE)
  {
    if (player.action == Action::HIT)
    {
      player.hand.push_back(deck.draw());
      if (player.getScore() > 21)
      {
        player.state = PlayerState::BUST;
        stopMask |= 1 << currentPlayer;
      }
    }
    else if (player.action == Action::STAND)
    {
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
```
To make the game more fun, 5 to 9 simulated players are included in the game. They have access to the same actions and make arbitrary decisions based on their hand's score following the basic strategy for a standard deck of 52 cards. The best action may vary if the hand contains an ace and a different decision tree is used if one is detected. 

A game log is displayed in the bottom left to inform the player of the card they drew as well their score. When all players are standing or bust, a winner is found following the rules for Blackjack as mentionned above.
A menu bar is also provided in the top letting players choose to start a new game.

<img src="https://user-images.githubusercontent.com/56483943/147889522-3f954cf2-a7af-4ca9-a921-60faa2f6b5a2.png" alt="V1 Screenshot" width="100%"/>

### Version 2
Version 2 features a rendered output of the board in OpenGL next to the game's log. This is handled by the BoardRenderer class which takes care of the OpenGL structures needed for drawing cards on the playing board. These are:
- A vertex array object with position and texture coordinate attributes for rendering a quad.
- A card back texture and a card front texture atlas (note we could have put the card back in the atlas).
- A frame buffer object with only a colour buffer as we do not need depth testing. The colour buffer is used as an image rendered in the GUI.
- A card shader rendering textured quads as playing cards.

The initial board layout is as follows: On the bottom are Player 1's cards and on the top are the other player's hands.
<img src="https://user-images.githubusercontent.com/56483943/148090363-be90e133-6e38-4461-a3db-e1b98a0b24ad.png" alt="V2 Screenshot" width="100%"/>

Because of the relatively static nature of board state, it is rendered only on specific occasions rather than at every frame. The occasions for rendering are: 
- A new game is started
- A new turn has started
- Player 1 has lost and all cards are revealed
- A player has gone bust and reveals their cards
- Player 1 wants to cheat and see all other cards on the board
- All players are either bust or standing

```cpp
// refresh board 
boardFramebuffer.bind();
glClearColor(0.0f, 0.29921f, 0.13437f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);

float cardScale = 0.3f;
cardShader.setFloat("cardScale", cardScale);
Vector2 stride = { 1.2f * cardScale, 0.0f }, cardPosition = { -0.8f, -0.65f };
// render player 1's hand
for (auto& card : blackjack->players.front().hand)
{
  cardShader.setVec2("cardPosition", cardPosition);
  drawCard(card);
  cardPosition += stride;
}

// render other players' hands ...
```
The above code snippet shows how the board is rendered. The framebuffer is bound and cleared with the dark green colour and shader uniforms are set with values determined empirically to scale and position the rendered card such that they all fit on the board. Because depth testing is disabled, cards will be rendered on top of each other and give the stacking effect for free. Because the board is rendered in 2D, there is no need to apply Model, View or Projection transforms and coordinates are passed to the shader already in normalized device coordinates (in range [-1,1]²).

Rendering the card fronts from a single texture is achieved by applying an affine transformation to the quad's UV texture coordinates depending on the rank and suite of the card to be rendered:
```cpp
void BoardRenderer::drawCard(uint32_t cardId)
{
  Vector2 texOffset = 
    ATLAS_CARD_COORDINATES[cardId % CARD_RANKS] + ATLAS_SUITE_OFFSETS[(cardId / CARD_RANKS) & 3]; // % 4

  cardAtlas.bind(0);
  cardShader.setVec4("texTransform", { 1.0f / 14.0f, 0.25f, texOffset[0], texOffset[1] });
  
  glDrawElements(GL_TRIANGLES, sizeof(QUAD_VERTICES) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}
```
![cardfront](https://user-images.githubusercontent.com/56483943/148094518-e3ea94a1-4b5d-4541-be0e-298f29e6e71e.jpg)
Texture obtained from: [https://thumbs.dreamstime.com/b/playing-cards-texture-11014760.jpg](https://thumbs.dreamstime.com/b/playing-cards-texture-11014760.jpg)

### Version 3
Version three introduces the choice of switching between playing against other players trying to get the highest score, as in versions 1 and 2, or playing like in a US casion against a dealer with a card in the hole. In the application's rules, the dealer does not draw when dealt an initial soft seventeen and the game is played with 4 playing card decks. The normal rules are followed: beating the dealer requires a hand with a higher score, tying if the hand is worth the same or losing when the score is smaller. A natural 21 from the player beats the dealer if the latter also gets 21. A natural 21 from the dealer can only be tied with another player's natural. 

Other players will choose to either hit or draw depending on their hand's score and the dealer's revealed card following the [basic strategy for playing with 4 to 8 deck of cards](https://en.wikipedia.org/wiki/Blackjack#Basic_strategy).

<img src="https://user-images.githubusercontent.com/56483943/148238338-41509c23-15f1-43e3-9a83-80993409e3d8.png" alt="Against the dealer" width="100%"/>
<img src="https://user-images.githubusercontent.com/56483943/148238457-ea419fce-3461-4199-a2e3-982ed5b085d2.png" alt="Against other players" width="100%"/>

## Libraries used
- GLFW window API
- GLAD OpenGl loader
- ImGui immediate mode gui
- Thomath personal 3D maths library
