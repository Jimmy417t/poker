#include "Deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() {
    reset();
}

void Deck::reset() {
    cards.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            cards.push_back(Card(static_cast<Suit>(s), static_cast<Rank>(r)));
        }
    }
    nextCardIndex = 0;
}

void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
    nextCardIndex = 0;
}

Card Deck::dealCard() {
    if (nextCardIndex >= cards.size()) {
        reset();
        shuffle();
    }
    return cards[nextCardIndex++];
}

int Deck::remainingCards() const {
    return cards.size() - nextCardIndex;
}