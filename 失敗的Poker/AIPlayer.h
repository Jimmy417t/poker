#pragma once

#include "Player.h"

class AIPlayer : public Player {
private:
    int getHandStrength();
    
public:
    AIPlayer(std::string n, int startingChips);
    
    Action takeTurn(int currentBet, int minRaise) override;
    int getRaiseAmount(int currentBet, int minRaise) override;
};
