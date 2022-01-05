# BBC Software Engineering Graduate Scheme Technical Task

## Overview
The given task is to make a program that plays the game of Blackjack. The basic scenario is quite straightforward: simulate a deck of 52 cards and draw cards to get as close to 21 as possible.

The game functions by letting players perform different actions, like hitting or standing, which may change their state. A player is either in a playing, bust or standing state. Bust and standing players can no longer perform any actions and are waiting for every other player to be either bust or standing. When this condition is met, the winner or winners are determined following Blackjack rules: The player with the highest score wins. Players with the same high score are tied. The exception to this rule is when the highest score is 21 in which case players with a "natural blackjack" (21 in their starting hand) take precedence over players with the same score but with more than two cards.

In Casinos, players bet money on whether their hand will beat the dealer's and often have more than one deck of 52 cards used (up to 8). The dealer plays after all other players are either standing or bust by repeatedly hitting until their hand is worth at least 17.

To play the game, download the executable. Make sure to place it in the same directory as the two textures _(cardback.jpg, cardfront.jpg)_ and two shaders _(card.vert, card.frag)_. Run it on the command line with an integer argument for the number of players (5 to 9).
```
C:\Blackjack.exe 5
```
## Design and implementation
I have chosen to implement this application in C++ as this is the language I am the most comfortable in and allows me to add graphics to the application.

### Version 1
Cards are uniquely identified by an unsigned integer id. Using the modulus operator and integer division, it's suite and rank can easily be retrieved. For instance with the set of suites { hearts, diamonds, clubs, spades }, the card with id 32 is a 6 of clubs (32 % 13 = 6 and 32 / 13 = 2 where the third suite is clubs). This allows for flexibility in the number of decks the game can use (Blackjack is commonly played with 1 to 8 decks).

A deck of cards is modeled by an array of N x 52 cards and is shuffled by repeatedly swapping two cards at random positions in the deck. A player's hand consists of an STL vector of unsigned integers which expansd whenever a card is drawn. Computing player scores is achieved by looping over the hand's values and adding the card's score by deducing it's rank which is used as an index into a 13 element array of scores. Aces need to be handled a little differently, as they can represent a value of 11 or 1 depending on the hand of the player. The algorithm initially adds them to the score as 11 whilst also keeping count of them. To count them without branching, we bit-shift 1 to the left by the card's rank to get a power of two (eg: 16 % 13 = 3, 1 << 3 = 2^3 or 1000 in binary). This is then bit-anded with the ace's binary encoding of 1 << 0 or simply 1 which is added to the aces tally. If the score is greater than 21 and there are aces, we can subtract 10 to the score to effectively turn them into 1 valued aces.
```cplusplus
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

The initial solution consist of a simple GUI with options to either Hit or Stand based on the player's current hand as well as a basic AI oponents that with the same set of actions. Players can start new games by interacting with a simple menu bar.

<img src="https://user-images.githubusercontent.com/56483943/147889522-3f954cf2-a7af-4ca9-a921-60faa2f6b5a2.png" alt="V1 Screenshot" width="100%"/>
Screenshot of application V1, featuring a text log displaying game events and scores as players make their decisions. Simulated oponents use the basic strategy for the number of decks used in the game. 

### Version 2
Version 2 features a rendered output in OpenGL of the board next to the game's log. This is handled by the BoardRenderer class which takes care of the OpenGL structures needed for drawing cards on the playing board. These are:
- A vertex array object with position and texture coordinate attributes for rendering a quad.
- A card back texture and a card front texture atlas (note we could have put the card back in the atlas).
- A frame buffer object with only a colour buffer as we do not need depth testing. The colour buffer is used as an image rendered in the GUI.

The initial board layout is as follows: On the bottom are Player 1's cards and on the top are the other player's hands.
<img src="https://user-images.githubusercontent.com/56483943/148090363-be90e133-6e38-4461-a3db-e1b98a0b24ad.png" alt="V2 Screenshot" width="100%"/>

The board is rendered every time it is updated. The occasions for update are: 
- A new game is started
- A new turn has started
- Player 1 has lost and all cards are revealed
- A player has gone bust and reveals their cards
- Player 1 wants to cheat and show all hands
- All players are either bust or standing and the winner(s) is decided

```cplusplus
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

// render other players' hands
...
```
The above code snippet shows how the board is rendered. The buffer is bound and cleared with the dark green colour and shader uniforms are set. These affect the scale and position of the rendered card. Because depth testing is disabled, cards will be rendered on top of each other and give the stacking effect for free. Furthermore, switching between showing or hiding the other hands is simply a case of changing the texture used as the new cards will be rendered directly on top of the previous ones. Because the board is rendered in 2D, there is no need to apply Model, View or Projection transforms and coordinates are passed to the shader already in normalized device coordinates.

Rendering the card fronts from a single texture is achieved by transforming the quad's UV texture coordinates depending on the card to be rendered:

```cpluplus
// given a card scale and a card position, build an affine transform for the card's texture coordinates
 static Vector2 texScale = { 1.0f / 14.0f, 0.25f };
 Vector2 texOffset = ATLAS_CARD_COORDINATES[cardId % 13] + ATLAS_SUITE_OFFSETS[(cardId / 13) & 3];
 cardAtlas.bind(0);
 cardShader.setVec4("texTransform", { texScale[0], texScale[1], texOffset[0], texOffset[1] });
 glDrawElements(GL_TRIANGLES, sizeof(QUAD_VERTICES) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
```
![cardfront](https://user-images.githubusercontent.com/56483943/148094518-e3ea94a1-4b5d-4541-be0e-298f29e6e71e.jpg)
Texture obtained from: [https://thumbs.dreamstime.com/b/playing-cards-texture-11014760.jpg](https://thumbs.dreamstime.com/b/playing-cards-texture-11014760.jpg)

### Version 3
In version three you can switch between playing against other players trying to get the highest score, or play like in a US casion against a dealer with a card in the hole. In this version, the dealer does not draw when dealt an initial soft seventeen and the game is played with 4 playing card decks. The normal rules are followed: beating the dealer requires a hand with a higher score, tying if the hand is worth the same or losing when the score is smaller. A natural 21 from the player beats the dealer if the latter also gets 21. A natural 21 from the dealer can only be tied with another player's natural. Other players will choose to either hit or draw based on the dealer's revealed card following the [basic strategy for playing with 4 to 8 deck of cards](https://en.wikipedia.org/wiki/Blackjack#Basic_strategy).
<img src="https://user-images.githubusercontent.com/56483943/148238338-41509c23-15f1-43e3-9a83-80993409e3d8.png" alt="Against the dealer" width="100%"/>
<img src="https://user-images.githubusercontent.com/56483943/148238457-ea419fce-3461-4199-a2e3-982ed5b085d2.png" alt="Against other players" width="100%"/>
