#pragma once

#include <vector>
#include "Player.h"
#include "AIPlayer.h"
#include "Deck.h"
#include "Card.h"
#include "HandRank.h"

class Game {
private:
    std::vector<Player*> players;
    Deck deck;
    std::vector<Card> communityCards;
    int pot;
    int currentBet;
    int dealerIndex;
    int smallBlind;
    int bigBlind;
    
    void dealHoleCards();
    void dealFlop();
    void dealTurn();
    void dealRiver();
    void bettingRound();
    void showdown();
    HandRankResult getBestHand(Player* player);
    void awardPot(Player* winner);
    void collectBets();
    void resetRound();
    void printCommunityCards() const;
    void printPlayerStatus() const;
    
public:
    Game();
    ~Game();
    void addPlayer(Player* p);
    void start();
    void run();
};
