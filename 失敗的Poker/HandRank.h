#pragma once

#include <vector>
#include "Card.h"

enum class HandType {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

struct HandRankResult {
    HandType type;
    std::vector<int> tieBreakers;  // 用於比牌
};

class HandRank {
public:
    static HandRankResult evaluate(const std::vector<Card>& cards);
    static int compare(const HandRankResult& a, const HandRankResult& b);
    
private:
    static bool isFlush(const std::vector<Card>& cards);
    static bool isStraight(std::vector<int> ranks);
    static std::vector<int> getRankCounts(const std::vector<Card>& cards);
    static std::vector<int> getSortedRanks(const std::vector<Card>& cards);
};
