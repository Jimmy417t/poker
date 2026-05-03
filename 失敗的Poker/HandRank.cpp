#include "HandRank.h"
#include <algorithm>
#include <map>

std::vector<int> HandRank::getSortedRanks(const std::vector<Card>& cards) {
    std::vector<int> ranks;
    for (const auto& card : cards) {
        ranks.push_back(static_cast<int>(card.getRank()));
    }
    std::sort(ranks.begin(), ranks.end(), std::greater<int>());
    return ranks;
}

std::vector<int> HandRank::getRankCounts(const std::vector<Card>& cards) {
    std::vector<int> ranks = getSortedRanks(cards);
    std::vector<int> counts(15, 0);
    for (int r : ranks) {
        counts[r]++;
    }
    return counts;
}

bool HandRank::isFlush(const std::vector<Card>& cards) {
    Suit firstSuit = cards[0].getSuit();
    for (const auto& card : cards) {
        if (card.getSuit() != firstSuit) return false;
    }
    return true;
}

bool HandRank::isStraight(std::vector<int> ranks) {
    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());
    
    if (ranks.size() < 5) return false;
    
    // 檢查連續 5 張
    for (size_t i = 0; i <= ranks.size() - 5; i++) {
        bool straight = true;
        for (int j = 0; j < 4; j++) {
            if (ranks[i + j + 1] != ranks[i + j] + 1) {
                straight = false;
                break;
            }
        }
        if (straight) return true;
    }
    
    // 特殊情況：A,2,3,4,5
    if (ranks[0] == 14 && ranks[1] == 2 && ranks[2] == 3 && ranks[3] == 4 && ranks[4] == 5) {
        return true;
    }
    
    return false;
}

HandRankResult HandRank::evaluate(const std::vector<Card>& cards) {
    HandRankResult result;
    std::vector<int> ranks = getSortedRanks(cards);
    std::vector<int> counts = getRankCounts(cards);
    bool flush = isFlush(cards);
    bool straight = isStraight(ranks);
    
    // 找出各種牌型
    int four = 0, three = 0, pair = 0;
    std::vector<int> pairs;
    
    for (int i = 2; i <= 14; i++) {
        if (counts[i] == 4) four = i;
        if (counts[i] == 3) three = i;
        if (counts[i] == 2) {
            pair++;
            pairs.push_back(i);
        }
    }
    
    // 同花順
    if (flush && straight) {
        // 皇家同花順
        if (ranks[0] == 14 && ranks[4] == 10) {
            result.type = HandType::ROYAL_FLUSH;
            return result;
        }
        result.type = HandType::STRAIGHT_FLUSH;
        result.tieBreakers = ranks;
        return result;
    }
    
    // 鐵支
    if (four) {
        result.type = HandType::FOUR_OF_A_KIND;
        result.tieBreakers = {four};
        for (int r : ranks) {
            if (r != four) {
                result.tieBreakers.push_back(r);
                break;
            }
        }
        return result;
    }
    
    // 葫蘆
    if (three && pair >= 1) {
        result.type = HandType::FULL_HOUSE;
        result.tieBreakers = {three, pairs[0]};
        return result;
    }
    
    // 同花
    if (flush) {
        result.type = HandType::FLUSH;
        result.tieBreakers = ranks;
        return result;
    }
    
    // 順子
    if (straight) {
        result.type = HandType::STRAIGHT;
        // 處理 A,2,3,4,5 的特殊情況
        if (ranks[0] == 14 && ranks[4] == 2) {
            result.tieBreakers = {5};
        } else {
            result.tieBreakers = {ranks[0]};
        }
        return result;
    }
    
    // 三條
    if (three) {
        result.type = HandType::THREE_OF_A_KIND;
        result.tieBreakers = {three};
        for (int r : ranks) {
            if (r != three) result.tieBreakers.push_back(r);
            if (result.tieBreakers.size() == 3) break;
        }
        return result;
    }
    
    // 兩對
    if (pair >= 2) {
        result.type = HandType::TWO_PAIR;
        std::sort(pairs.begin(), pairs.end(), std::greater<int>());
        result.tieBreakers = pairs;
        for (int r : ranks) {
            if (r != pairs[0] && r != pairs[1]) {
                result.tieBreakers.push_back(r);
                break;
            }
        }
        return result;
    }
    
    // 一對
    if (pair == 1) {
        result.type = HandType::ONE_PAIR;
        result.tieBreakers = {pairs[0]};
        for (int r : ranks) {
            if (r != pairs[0]) result.tieBreakers.push_back(r);
            if (result.tieBreakers.size() == 4) break;
        }
        return result;
    }
    
    // 高牌
    result.type = HandType::HIGH_CARD;
    result.tieBreakers = ranks;
    return result;
}

int HandRank::compare(const HandRankResult& a, const HandRankResult& b) {
    if (a.type != b.type) {
        return static_cast<int>(a.type) - static_cast<int>(b.type);
    }
    
    for (size_t i = 0; i < a.tieBreakers.size() && i < b.tieBreakers.size(); i++) {
        if (a.tieBreakers[i] != b.tieBreakers[i]) {
            return a.tieBreakers[i] - b.tieBreakers[i];
        }
    }
    return 0;
}