/* Uses ADT of queue and stack in card game in which a varying amount of players 
decided by user are dealed seven cards in a dealer stack of cards in the range of 1-13.
There are 2 multiplied by the player size amount of each card in the dealers deck.
The game starts with the dealing of the card then followed by the placement of a card in the
discard stack.
The main game loop follows a round-robin order which calls a certain list of actions to be
done by a player class which is the active player. The player places his card in the discard stack and
compares his card value with the value given by the top of the discard stack and if his card is 
greater than the top card, the player draws two cards from the dealers deck; if they are equal 
the player draws one from the deal stack and if the player card is less than the top card
draw none. This repeats for each player until either a player runs out of cards in which they win the
game; or until the deal stack runs out of cards and the discard stack has one or fewer cards in it
which then makes a tie game.
*/

#include "queue.h"
#include "stack.h"
#include <iostream>
#include <random>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ProtoTypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 
    Desc: A container for the traits of a player like their id number in game; as well as
    their queue which works as a deck for the game.
*/
struct Player;

/*
Preconditions: Take in reference to int variable which will store the amount of players
in the game.
Postconditions: Will return a Player*[] of playerCount size which will have num member values
of 1-playerCount. Also will store player count requested in playerCount reference.
*/
Player* askPlayerCount(int& playerCount);

/*
Preconditions: Take in reference to stack variable to display values of
Postconditions: Will display each value stored in stack without loss of data
in array form (Ex. [1,2,3,4,5]). Going from top to bottom.
*/
void display(stack &stackToIndex);

/*
Preconditions: Take in reference to stack variable to shuffle int data of.
As well as the size of the stack variable to shuffle.
Postconditions: Will flip two different index values with eachother
in a array copy of the currentStack [2000-3250] amount of times. 
And will store state in currentStack after. Keep in order, first is first.
*/
void shuffleStack(stack& currentStack, int size);

/*
Preconditions: Take in amount of players for game dealer hand to pull for
Postconditions: Will return a stack which is a shuffled deck of cards 1-13
which occur each 2*playerAmount times (calls shuffleStack).
*/
stack* createDealStack(int playerAmount);

/*
Preconditions: Takes in reference to dealer, and players array and the 
amount of players currently playing.
Postconditions: Will give each player in 0-playerCount of players 
array 7 cards each from dealers deck in robin order (I.E. player one
gets a card, then second, then third, then back to first).
*/
void dealCards(stack*& dealer, Player*& players, int playerCount);

/*
Preconditions: Take in reference to pointer to activePlayer as well as 
current deal stack and discard stack. As well as a reference to
the gameActive status of the game loop.
Postconditions: Will compare if front card in activePlayer queue is 
greater than the top card in the discard stack, in which case activePlayer
draws 2 cards; if top card is equal to top card in discard then only draw one;
if less though, do nothing. If cards run out in dealStack, replace with cards
in discard stack if discard stack is more than one card in length. Else, 
call a game tie.
*/
void compare(Player*& activePlayer, stack*& dealStack, stack*& discardStack, bool& gameActive);

/*
Preconditions: Take in amount of players for game dealer hand to pull from.
Also player in which will receive the card and the discard stack.
Postconditions: Place next in dealer stack in player's queue. If dealer
is empty and discard is empty or less than one or equal to one in size
then return a tie. Else if more than one, set dealer equal to discard
and set discard equal to nothing plus the first on the dealers new stack.
Return boolean of if game is still active after comparison.
*/
bool getCard(stack *&dealer, Player *&player, stack *&discard);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END ProtoTypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct Player{
    queue* currentQueue = new queue(); // Holds players cards in-game
    int num; // Id-number of 1-playerCount
};

Player* askPlayerCount(int& playerCount)
{
    // Clear from previous prompts in console
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    do
    {

        // Ask player amount
        std::cout << "How many players will be playing? ";
        std::cin >> playerCount;

        // Check if less than 0 players
        if(playerCount <= 0){
            std::cout << "Sorry, cannot be less than 1 player. Try again! \n\n";
        }

        // Clear non-int values in buffer
        std::cin.clear();
        std::cin.ignore();

    } while (playerCount <= 0); // Keep going if less than 0

    // Make temp player array in order to set player nums to correct values during initialization
    Player* tmpPlayerList = new Player[playerCount];

    // Step through each and make player have num of 1-playerCount
    for(int i = 0; i < playerCount; i++){
        tmpPlayerList[i].num = i+1;
    }

    return tmpPlayerList; // Return pointer to allocated data

}

void display(stack &stackToIndex) // Display stack values 
{
    stack tmpStack; // Holds reversed stackToIndex in order to pop in normal order as pop last->first

    while (!stackToIndex.isEmpty()){ // Place into temp
        int tmp = stackToIndex.pop();
        std::cout << tmp << ((!stackToIndex.isEmpty()) ? "," : ""); // Print current value
        tmpStack.push(tmp);
    }
    std::cout << "]\n\n"; // Close visual array

    while (!tmpStack.isEmpty()){ // Print the temp in reverse (pop goes last to first)
        stackToIndex.push(tmpStack.pop());
    }

}

void shuffleStack(stack& currentStack, int size)
{
    std::random_device rand;   // Making seeded gen
    std::mt19937 seed(rand()); // Seeding with quicker gen with seed of rand gen
    std::uniform_int_distribution<int> randInt(0, (size-1)); // Gives random number in toShuffle's index
    std::uniform_int_distribution<int> randShuffleAmnt(2000, 3250); // Amount of times to shuffle deck
    int toShuffle[size];

    currentStack.flip(); // Make first last so when popping first is first

    for(int i = 0; i < size; i++){ // Move values over to array
        toShuffle[i] = currentStack.pop(); // Goes last to first
    }
    
    int timesToShuffle = randShuffleAmnt(seed);
    int first; // Holds index one for swap
    int second; // Holds index two for swap
    int tmp; // Holds value of first index for swapping to second
    
    for(int i = 0; i < timesToShuffle; i++){
    
    // Get indexes to flip
    first = randInt(seed);
    do{
        second = randInt(seed);
    } while(second == first); // Make sure they aren't same index

    tmp = toShuffle[first]; // Save first
    toShuffle[first] = toShuffle[second]; // Swap first
    toShuffle[second] = tmp; // Swap second with tmp (old first)

    }
    
    // Remove one so size is equal to amount of "live" indexes
    size--;

    // Display values being placed in stack
    std::cout << "\nDisplaying before deal stack values: \n[";
    for (int i = 0; i <= size; i++) // Move values over to stack after shuffle
    {                                                   
        std::cout << toShuffle[i] << ((i == size) ? "]\n\n" : ","); // Print front to back (last to first in index)
        currentStack.push(toShuffle[i]);
    }


}

stack* createDealStack(int playerAmount)
{
    stack* tmp = new stack(); // Holds stack for creation in function

    for(int i = 1; i <= 13; i++){ // Get value 1-13
        for(int d = 0; d < playerAmount*2; d++){ // Add that value playerAmount*2 times
            tmp->push(i);
        }
    }

    // Shuffle stack (new stack size is amount of values per value, times amount of values (2*13*playerAmount))
    shuffleStack(*tmp, playerAmount*2*13);

    return tmp;

}

void dealCards(stack*& dealer, Player*& players, int playerCount) // Ask if changing to * does anything in this case (makes it a copy?)
{
    int robin = 0;
    for(int i = 0; i < playerCount*7; i++){ // For each player in playerCount
        players[robin].currentQueue->enqueue(dealer->pop());
        robin = (robin+1)%playerCount;
    }

}

bool getCard(stack*& dealer, Player*& player, stack*& discard)
{
    try{
        int tmp = dealer->pop();
        std::cout << tmp;
        player->currentQueue->enqueue(tmp);
    
    }
    catch(std::runtime_error &runtimeError){ // If ran out of cards in dealers hand
        if(!discard->isEmpty()){ // If still have at least one in discard stack

            int tmp = discard->pop(); // remove one then check if empty

            if(discard->isEmpty()){ // If empty

                // Game tie
                std::cout << tmp << " \n\nNo-one won it was a tie !! \n\n"; // End prompt
                discard->push(tmp); // Re-add removed value
                return false; // Game over

            }

            // Re-add removed value
            discard->push(tmp);
            *dealer = *discard; // set deal equal to discard stack

            delete discard; // Clear discard stack

            discard = new stack(); // Make it a new one
            discard->push(dealer->pop()); // Place head card out of deal

            tmp = dealer->pop();
            std::cout << tmp;
            player->currentQueue->enqueue(tmp);
            
        }
        else{ // Game tie
            std::cout << "\nNo-one won it was a tie !! \n\n"; // End prompt
            return false; // Game over
        }
    }
    return true;
}

void compare(Player*& activePlayer, stack*& dealStack, stack*& discardStack, bool& gameActive) 
{
    
    // Get top of discard and next in player deck
    int top = discardStack->pop(); 
    int next = activePlayer->currentQueue->dequeue();

    // Place both cards in discard stack
    discardStack->push(top);
    discardStack->push(next);

    // Main prompt
    std::cout << "Player " << activePlayer->num << " plays " << next << " against the top card " << top << " and draws "; 

    // Check players action
    if(next < top){ // Grab 2 from dealer

        gameActive = getCard(dealStack, activePlayer, discardStack);
        std::cout << ", ";

        gameActive = getCard(dealStack, activePlayer, discardStack);

    }
    else if(next == top){ // Grab one from dealer

        gameActive = getCard(dealStack, activePlayer, discardStack);

    }
    else{ // Grab none from dealer

        std::cout << "nothing...";

    }
    std::cout << "\n";
}

int main()
{   
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INITIALIZE GAME VARIABLES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // Ask amout of player amount and store in value for later conditional use
    int gamePlayersAmount;

    // Get player list of set of players and set gamePlayersAmount
    Player *currentPlayers = askPlayerCount(gamePlayersAmount);

    // Initialize defaults of game members
    stack* dealStack = createDealStack(gamePlayersAmount); 
    stack* discardStack = new stack();

    // Deal cards to players
    dealCards(dealStack, currentPlayers, gamePlayersAmount); // Give each player 7 cards
    discardStack->push(dealStack->pop()); // Now place next card on top of discard then start game

    //Game on flag
    bool gameActive = true;
    int robinOrder = 0;

    // Keep playing game flag
    bool programActive = true;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GAME LOOP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    while(programActive){

        while(gameActive){ // Start game instance

            // Get player for turn and declare whos going
            Player *activePlayer = &currentPlayers[robinOrder];
            std::cout << "\nPlayer " << robinOrder+1 << " turn: \n"; // Declare player

            compare(activePlayer, dealStack, discardStack, gameActive); // Main logic function

            // If player runs out of cards by end of turn
            if(activePlayer->currentQueue->isEmpty()){ // If activePlayer Win

                std::cout << "\n\nPlayer " << activePlayer->num << " Won! \n\n";
                gameActive = false;

                // After display values
                std::cout << "\nDisplaying final deal stack values: \n[";
                display(*dealStack);
            }

            robinOrder = (robinOrder+1)%gamePlayersAmount; // Iterate through players and go back to first after last
        }

        // Ask if another game is wished to be played
        std::cout << "\nDo you wish to play again? (1 = yes; 0 = no): ";
        std::cin >> gameActive;
        
        // \/ Need to dealloc data before new game or end game so do now \/
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Cleanup~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        delete discardStack;
        delete dealStack;

        for(int i = 0; i < gamePlayersAmount; i++){
            delete currentPlayers[i].currentQueue; // Clear players decks
        }

        delete[] currentPlayers; // Clear player list
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        if(!gameActive){
            // Set Outside game loop to close program
            programActive = false;
        }
        else{

            // Reset game variables
            robinOrder = 0;
            currentPlayers = askPlayerCount(gamePlayersAmount);
            dealStack = createDealStack(gamePlayersAmount);
            discardStack = new stack();

            // Deal cards to players
            dealCards(dealStack, currentPlayers, gamePlayersAmount);
            discardStack->push(dealStack->pop()); // Now place next card on top of discard then start game

        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}
