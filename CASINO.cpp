#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <string>
#include <windows.h>
#include "CASINO.h" 

using std::cout;
using std::endl;


// Constant size of deck of cards and numbers on roulette
const int num_cards = 52;
const int rouletteNums = 37;

// Player Functions
// Setter
void Player::setBalance(int amount) {
    balance = amount;
}
// Getter
int Player::getBalance() {
    return balance;
}
// Setter
void Player::setUsername(string name){
    username = name;
}
// Getter
string Player::getUsername(){
    return username;
}


// Initalize the deck of cards with correct ranks and suits
// Suits arent being used but could be used in future improvements
void initializeDeck(Card deck[])
{
    string suits[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string ranks[13] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    int curCard = 0;
    for (int suitIdx = 0; suitIdx < 4; suitIdx++) {
        for (int rankIdx = 0; rankIdx < 13; rankIdx++){
            deck[curCard].suit = suits[suitIdx];
            deck[curCard].rank = ranks[rankIdx];
            curCard++;
        }
    }
}

// Shuffling the deck of cards to not have the same cards show up
void shuffleDeck(Card deck[])
{
    srand(time(NULL));
    int shuffleAmount = rand()%(43) + 10;
    for (int i = 0; i < shuffleAmount; i++) {
        int randCard = rand()%(43) + 10;
        Card temp = deck[i];
        deck[i] = deck[randCard];
        deck[randCard] = temp;
    }
}

// Giving card to a user or dealer and incrementing top to keep track of what would be the next card distributed
Card giveCard(Card deck[], int &top) 
{
    Card temp = deck[top];
    top++;
    return temp;
}

// Making a hand for the dealer and having to hit if the sum is below 17 like real life
int dealerBlackjack(Dealer &dealer, Card deck[], int &top)
{
    int dealerHandSize = 2;
    int dealerSum = 0;
    for (int i = 0; i < 2; i++){
        dealer.hand[i] = giveCard(deck, top);
    }
    for (int i = 0; i < dealerHandSize; i++){
        if (dealer.hand[i].rank == "Ace" || dealer.hand[i].rank == "King" || dealer.hand[i].rank == "Queen" || dealer.hand[i].rank == "Jack") {
            dealerSum += 10;
        }else dealerSum += stoi(dealer.hand[i].rank);
    }
    while (dealerSum < 17) {
        dealer.hand[dealerHandSize] = giveCard(deck, top);
        if (dealer.hand[dealerHandSize].rank == "Ace" || dealer.hand[dealerHandSize].rank == "King" || dealer.hand[dealerHandSize].rank == "Queen" || dealer.hand[dealerHandSize].rank == "Jack") {
            dealerSum += 10;
        } else {
            dealerSum += stoi(dealer.hand[dealerHandSize].rank);
        }
        dealerHandSize++;
    }
    return dealerSum;
}

// The game of Blackjack
int playerBlackjack(Player &player1, Dealer dealer1, Card deck[], int &top)
{
    cout << "---Welcome to Blackjack: The most popular casino game!---" << endl;
    int betAmount;
    cout << "Your current balance: " << player1.getBalance() << endl
         << "How much do you want to bet? ";
    cin >> betAmount;
    // Checking if valid bet amount
    if (betAmount > player1.getBalance() || betAmount <= 0) {
        cout << "Invalid bet amount." << endl;
        return player1.getBalance(); // Return the current balance without making any changes
    }
    // Giving out two cards to user
    for (int i = 0; i < 2; i++){
        player1.hand[i] = giveCard(deck, top);
    }
    
    // Game 
    int playerHandSize = 2;
    int handSum = 0;
    cout << "Your hand: " << endl;
    // Summing up what the user has in their initial hand
    for (int i = 0; i < 2; i++) {
        if (player1.hand[i].rank != "") {
            cout << player1.hand[i].rank << " of " << player1.hand[i].suit << endl;
            if (player1.hand[i].rank == "Ace" || player1.hand[i].rank == "King" || player1.hand[i].rank == "Queen" || player1.hand[i].rank == "Jack") {
                handSum += 10;
            }else handSum += stoi(player1.hand[i].rank);
        }
    }
    bool continuePlaying = true;
    
    while (continuePlaying){
        string play;
        cout << "Your total: " << handSum << endl
             << "Would you like to hit or stay? (Hit/Stay) " << endl;
        cin >> play;
        // Giving user another card if user wants to Hit
        if (play == "Hit" && (handSum < 21)){
            player1.hand[playerHandSize] = giveCard(deck, top);
            if (player1.hand[playerHandSize].rank == "Ace" || player1.hand[playerHandSize].rank == "King" || player1.hand[playerHandSize].rank == "Queen" || player1.hand[playerHandSize].rank == "Jack") {
                    handSum += 10;
                }else handSum += stoi(player1.hand[playerHandSize].rank);
            playerHandSize++;
       }

       // Ending the hit or stay loop if the user has gone over or equal to 21 or if they want to Stay or if hand size if 5 (rule in real game)
       if (play == "Stay" || handSum > 22 || handSum == 21 || playerHandSize == 5){
        continuePlaying = false;
       }
    }
    // Retrieving what the dealer has
    int dealerTotal = dealerBlackjack(dealer1, deck, top);
    // Letting user know what the two hands are
    cout << "You have: " << handSum << endl << "The dealer has: " << dealerTotal << endl;
    // Going through all win conditions
    if (handSum > 21) {
        player1.setBalance(player1.getBalance() - betAmount);
        cout << "Bust! You lose."<< endl 
             << "Your new balance: $" << player1.getBalance() << endl << endl;
    } else if (handSum == 21 || (handSum < 21 && dealerTotal > 21)){
        player1.setBalance(player1.getBalance() + betAmount);
        cout << "***Blackjack! You win.***" << endl 
             << "Your new balance: $" << player1.getBalance() << endl << endl;
    } else if (handSum < 21 && handSum < dealerTotal) {
        player1.setBalance(player1.getBalance() - betAmount);
        cout << "Dealer wins." << endl 
             << "Your new balance: $" << player1.getBalance() << endl << endl;
    } else {
        cout << "It's a tie! Your balance stays the same." << endl << endl;
    }
    // Return the updated balance after the game
    return player1.getBalance(); 
}

// Initializing the options of a roulette board
void initializeRoulette(Roulette rouBoard[])
{
    // Each option has a value and a color 
    rouBoard[0].value = 0;
    rouBoard[0].color = "Green";
    for (int i = 0; i < 36; i++) {
        rouBoard[i+1].value = i+1;
        if (i % 2 == 0 && (i >= 1 && i<=10)) {
            rouBoard[i+1].color = "Black";
        }
        else if ((i % 2 == 0 && (i >= 19 && i<=28)))
        {
            rouBoard[i+1].color = "Black";
        }
        else if ((i % 2 == 1 && (i >= 11 && i<=18)))
        {
            rouBoard[i+1].color = "Black";
        }
        else if ((i % 2 == 1 && (i >= 29 && i<=36)))
        {
            rouBoard[i+1].color = "Black";
        }
        else rouBoard[i+1].color = "Red";
    }
}

// Picking a random value from the roulette board
Roulette spinRoulette(Roulette rouBoard[]) 
{
    srand(time(NULL));

    // Generate a random index between 0 and 36 (inclusive)
    int randomIndex = rand() % 37;

    // Return the value and color of the randomly chosen index
    return rouBoard[randomIndex];
}

// The game where the user chooses if they want color or number bet and compares with spinRoulette
int playRoulette(Player &player1, Roulette rouBoard[])
{
    cout << "---Welcome to roulette! Just some classic roulette!---" << endl;
    int betAmount;
    int choice;

    cout << "Your current balance: " << player1.getBalance() << endl
         << "How much do you want to bet? ";
    cin >> betAmount;

    // Checking if valid bet amount
    if (betAmount > player1.getBalance() || betAmount <= 0) {
        cout << "Invalid bet amount." << endl;
        return player1.getBalance(); // Return the current balance without making any changes
    }
    // Determining users bet choice
    cout << "Which kind of bet would you like? " << endl
         << "1. Bet on a number " << endl
         << "2. Bet on a color " <<endl;
    cin >> choice;
    // Retrieving what the users bet will be compared to
    Roulette result = spinRoulette(rouBoard);
    // Getting and comparing the number the user would like to bet on
    if (choice == 1){
        int numberChosen;
        cout << "Enter the number you want to bet on (0-36 inclusive): ";
        cin >> numberChosen;

        if (numberChosen > 36 || numberChosen < 0){
            cout << "Invalid number selected " << endl;
            return player1.getBalance();
        }
        // Letting user know what the roulette ball landed on
        cout << "^^ The roulette ball landed on " << result.color << " " << result.value << "^^"<< endl;
        // Checking if user won the bet and updating balance accordingly
        if (numberChosen == result.value) {
            player1.setBalance(player1.getBalance() + (36*betAmount));
            cout << "***Congratulations! You Won.***" << endl 
                 << "Your new balance: $" << player1.getBalance() << endl << endl;
        } else {
            player1.setBalance(player1.getBalance() - betAmount);
            cout << "Sorry, you lost." << endl 
                 << "Your new balance: $" << player1.getBalance() << endl << endl;
        }
    // User betting on color 
    } else if (choice == 2) {
        string colorChosen;
        cout << "Enter the color you want to bet on (Red/Black/Green): ";
        cin >> colorChosen;
        if (colorChosen != "Red" && colorChosen != "Black" && colorChosen != "Green"){
            cout << "Invalid color selected " << endl;
            return player1.getBalance();
        }
        // Letting user know what the roulette ball landed on
        cout << "The roulette ball landed on " << result.color << " " << result.value << endl;
        // Checking if user won the bet and updating balance accordingly
        if (result.color == colorChosen) {
            // Increased payout for betting on Green since lower probability
            if (result.color == "Green"){
                player1.setBalance(player1.getBalance() + (36*betAmount));
            }else {
                player1.setBalance(player1.getBalance() + betAmount);
            }
            cout << "***Congratulations! You won.***" << endl 
                 << "Your new balance: $" << player1.getBalance() << endl << endl;
        }else {
            player1.setBalance(player1.getBalance() - betAmount);
            cout << "Sorry, you lost." << endl 
                 << "Your new balance: $" << player1.getBalance() << endl << endl;
            
        }

    }
    // Return the updated balance after the game
    return player1.getBalance(); 
}

// Game of Slots
int playSlots(Player& player1)
{
    // To display emojis
    SetConsoleOutputCP(CP_UTF8);
    cout << "---Welcome to slots where you can win in 8 ways! (Tic-Tac-Toe Style)---" << endl;
    
    int betAmount;
    cout << "Your current balance: " << player1.getBalance() << endl
         << "How much do you want to bet per roll? ";
    cin >> betAmount;
    
    // Checking if valid bet amount
    if (betAmount > player1.getBalance() || betAmount <= 0) {
        cout << "Invalid bet amount." << endl;
        return player1.getBalance(); // Return the current balance without making any changes
    }
    
    srand(time(NULL));
    // Unicode for: Watermelon Cherry Kiwi Bell 7
    string slotOptions[5] = {"|\U0001F349 ", "|\U0001F352 ", "|\U0001F514 ", "|\U0001F95D ", "| 7 "};
    string slots[3][3];
    bool playing = true;
    while (playing){
        // Create a new random slot generator everytime and printing it out for the user
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3 ; j++){
                int randomIndex = rand() % 5;
                slots[i][j] = slotOptions[randomIndex];
                cout  << slots[i][j];
            }
            cout << "|" << endl;
        }
        // Checking how many lines the user one
        int winMultiplier = slotsCheckWin(slots);
        // If none then the user did not win and updates balance
        if (winMultiplier == 0){
            player1.setBalance(player1.getBalance() - betAmount);
            cout << "You did not win." << endl 
                 << "Your new balance: $" << player1.getBalance() << endl; 
        } else {
            // This means user did win and balance is updated 
            player1.setBalance(player1.getBalance() + (winMultiplier*betAmount));
            cout << "***You won " << winMultiplier << " line(s)!***" << endl
                 << "Your new balance: $" << player1.getBalance();
        }
        // Ask if they want to spin again
        string choice;
        cout << endl << "Would you like to play again ('Y'/'N'): ";
        cin >> choice;
        // Exit loop
        if (choice == "N" || player1.getBalance() <= 0){
            playing = false;
        }
    }
    // Return updated balance after finishing the desired amount of spins
    return player1.getBalance();
}

// Checking how many lines the user has won in slots
int slotsCheckWin(string slots[3][3]){
    int winAmount = 0;
    // Check rows
    for (int i = 0; i < 3; i++){
        if (slots[i][0] == slots[i][1] && slots[i][1] == slots[i][2]){
            winAmount++;
        }
    }
    // Check columns
    for (int i = 0; i < 3; i++){
        if (slots[0][i] == slots[1][i] && slots[1][i] == slots[2][i]){
            winAmount++;
        }
    }
    // Check diagonals
    if (slots[0][0] == slots[1][1] && slots[1][1] == slots[2][2]) {
        winAmount++;
    }
    if (slots[0][2] == slots[1][1] && slots[1][1] == slots[2][0]) {
        winAmount++;
    }
    // Return the multiplier of their betAmount for the spin
    return winAmount;
}

int main() 
{
    // Creating player
    Player player1;
    string inputUsername;
    cout << endl << "---Welcome to Brandon's casino where the odds are in your favour (not really)---" 
         << endl << "Enter your username: ";
    // Setting username
    cin >> inputUsername;
    player1.setUsername(inputUsername);

    // Initializing the deck of cards and roulette board
    Card deck[num_cards];
    initializeDeck(deck);
    Roulette rouBoard[rouletteNums];
    initializeRoulette(rouBoard);

    // Asking how much money the user would like to start with
    int inputBalance;
    cout << "Greetings " << player1.getUsername() << "! How much money would you like to deposit: ";
    cin >> inputBalance;
    player1.setBalance(inputBalance);
    
    // Overall game loop
    bool looping = true;
    while (looping){
        // Checking if user balance is 0 because if its 0 then you cant bet
        if (player1.getBalance() == 0){
            string continuePlaying;
            cout << "Your balance is 0. Would you like to add more into your balance (Yes/No): "; 
            cin >> continuePlaying;
            // User is out of money and leaves casino
            if (continuePlaying == "No") {
                looping = false;
            } else {
                // User is depositing more and it is added to the original input balance to calcuate the overall loss/gain at the end
                int depositMore;
                cout << "How much more would you like to deposit: " << endl;
                cin >> depositMore;
                inputBalance += depositMore;
                player1.setBalance(depositMore);
            }
        }
        // Asking what user wants to play
        int gamechoice;
        cout << "What game would you like to play " << player1.getUsername() << "?" << endl 
             << "1. Roulette" << endl 
             << "2. Blackjack" << endl
             << "3. Slots" << endl 
             << "4. Quit" << endl 
             << "(1/2/3/4): ";
        cin >> gamechoice;
        // Choosing users game
        if (gamechoice == 1){
            // Creating Roulette Wheel
            playRoulette(player1, rouBoard);
        } else if (gamechoice == 2){
            Dealer dealer1;
            int top = 0;
            shuffleDeck(deck);
            playerBlackjack(player1, dealer1, deck, top);
        } else if(gamechoice == 3){
            playSlots(player1);
        } else if (gamechoice == 4) {
            looping = false;
        }else cout << "Invalid choice. Try again " << endl;
    }
    // End of game loop and the user is leaving the casino with a fun night
    // Tells users their overall gain or loss
    cout << "Thank you for visiting Brandon's casino " << player1.getUsername() << "!" << endl;
    if (inputBalance >= player1.getBalance()){
        cout << "You lost $" << (inputBalance - player1.getBalance());
    } else cout << "You won $" << (player1.getBalance() - inputBalance);
    
    return 0;
}
