# CPP_QueueStack_CardGame

Uses ADT of queue and stack in card game in which a varying amount of players 
decided by user are dealed seven cards in a dealer stack of cards in the range of 1-13.
There are 2 multiplied by the player size amount of each card in the dealers deck.
The game starts with the dealing of the card then followed by the placement of a card in the
discard stack.

<img src="https://github.com/Kingerthanu/CPP_QueueStack_CardGame/assets/76754592/cd552e57-2f81-493a-b581-84b373f06c02" alt="Cornstarch <3" width="95" height="99">

The main game loop follows a round-robin order which calls a certain list of actions to be
done by a player class which is the active player. The player places his card in the discard stack and
compares his card value with the value given by the top of the discard stack and if his card is 
greater than the top card, the player draws two cards from the dealers deck; if they are equal 
the player draws one from the deal stack and if the player card is less than the top card
draw none. This repeats for each player until either a player runs out of cards in which they win the
game; or until the deal stack runs out of cards and the discard stack has one or fewer cards in it
which then makes a tie game

![2024-01-1018-15-18_8-ezgif com-optimize](https://github.com/Kingerthanu/CPP_QueueStack_CardGame/assets/76754592/7e4d8f4a-ed06-4059-b40b-447b3f829fda)
