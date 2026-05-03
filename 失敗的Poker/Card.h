#pragma once

#include <iostream>
#include <string>

enum class Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
enum class Rank { TWO=2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

class Card {
private:
    Suit suit;
    Rank rank;
    
public:
    Card();
    Card(Suit s, Rank r);
    
    Suit getSuit() const;
    Rank getRank() const;
    
    std::string toString() const;
    
    bool operator==(const Card& other) const;
};