#pragma once

#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card> cards;
    int nextCardIndex;
    
public:
    Deck();
    void shuffle();
    Card dealCard();
    int remainingCards() const;
    void reset();
};
