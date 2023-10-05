#ifndef CASINO_H
#define CASINO_H
#include <string>
using namespace std;

struct Card {
    string rank;
    string suit;
};

class Player {
    private:
        string username;
        int balance;

    public:
        Card hand[5];

    void setBalance(int amount);
    int getBalance();
    void setUsername(string name);
    string getUsername();
};

class Dealer {
    public:
        Card hand[5];
};

void initializeDeck(Card deck[]);
void shuffleDeck(Card deck[]);
Card giveCard(Card deck[], int& top);
int dealerBlackjack(Dealer& dealer, Card deck[], int& top);
int playerBlackjack(Player& player1, Dealer dealer1, Card deck[], int& top);

struct Roulette {
    int value;
    string color;
};

void initializeRoulette(Roulette rouBoard[]);
Roulette spinRoulette(Roulette rouBoard[]);
int playRoulette(Player& player1, Roulette rouBoard[]);


int playSlots(Player& player1);
int slotsCheckWin(string slots[3][3]);


#endif