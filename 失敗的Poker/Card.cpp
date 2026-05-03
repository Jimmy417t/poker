#include "Card.h"

Card::Card() : suit(Suit::CLUBS), rank(Rank::TWO) {}

Card::Card(Suit s, Rank r) : suit(s), rank(r) {}

Suit Card::getSuit() const { return suit; }
Rank Card::getRank() const { return rank; }

std::string Card::toString() const {
    std::string suitStr, rankStr;
    
    switch (suit) {
        case Suit::CLUBS: suitStr = "♣"; break;
        case Suit::DIAMONDS: suitStr = "♦"; break;
        case Suit::HEARTS: suitStr = "♥"; break;
        case Suit::SPADES: suitStr = "♠"; break;
    }
    
    switch (rank) {
        case Rank::TWO: rankStr = "2"; break;
        case Rank::THREE: rankStr = "3"; break;
        case Rank::FOUR: rankStr = "4"; break;
        case Rank::FIVE: rankStr = "5"; break;
        case Rank::SIX: rankStr = "6"; break;
        case Rank::SEVEN: rankStr = "7"; break;
        case Rank::EIGHT: rankStr = "8"; break;
        case Rank::NINE: rankStr = "9"; break;
        case Rank::TEN: rankStr = "10"; break;
        case Rank::JACK: rankStr = "J"; break;
        case Rank::QUEEN: rankStr = "Q"; break;
        case Rank::KING: rankStr = "K"; break;
        case Rank::ACE: rankStr = "A"; break;
    }
    
    return rankStr + suitStr;
}

bool Card::operator==(const Card& other) const {
    return suit == other.suit && rank == other.rank;
}