#pragma once

#include <string>
#include <vector>
#include "Card.h"

enum class Action { FOLD, CALL, RAISE };

class Player {
protected:
    std::string name;
    int chips;
    std::vector<Card> hand;
    int currentBet;
    
public:
    Player(std::string n, int startingChips);
    virtual ~Player() = default;
    
    void receiveCard(Card c);
    void clearHand();
    void showHand() const;
    std::vector<Card> getHand() const;
    
    int getChips() const;
    void deductChips(int amount);
    void addChips(int amount);
    void setCurrentBet(int bet);
    int getCurrentBet() const;
    
    std::string getName() const;
    
    virtual Action takeTurn(int currentBet, int minRaise);
    virtual int getRaiseAmount(int currentBet, int minRaise);
    
    bool isActive() const;
};
