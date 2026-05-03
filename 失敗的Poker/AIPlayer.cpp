#include "AIPlayer.h"
#include <cstdlib>

AIPlayer::AIPlayer(std::string n, int startingChips) : Player(n, startingChips) {}

int AIPlayer::getHandStrength() {
    if (hand.size() < 2) return 0;
    
    int r1 = static_cast<int>(hand[0].getRank());
    int r2 = static_cast<int>(hand[1].getRank());
    bool sameSuit = (hand[0].getSuit() == hand[1].getSuit());
    
    // 對子
    if (r1 == r2) {
        if (r1 >= 14) return 10;      // AA
        if (r1 >= 12) return 9;       // KK, QQ
        if (r1 >= 10) return 7;       // TT, JJ
        return 5;                      // 小對子
    }
    
    int high = std::max(r1, r2);
    int low = std::min(r1, r2);
    
    // AK, AQ, AJ
    if (high == 14 && low >= 11) {
        return sameSuit ? 9 : 8;
    }
    
    // 其他高牌
    if (high >= 12) {
        return sameSuit ? 6 : 4;
    }
    
    // 普通牌
    return sameSuit ? 3 : 2;
}

Action AIPlayer::takeTurn(int currentBet, int minRaise) {
    int strength = getHandStrength();
    int toCall = currentBet - this->currentBet;
    
    std::cout << name << " (AI) 正在思考..." << std::endl;
    
    // 好牌：加注
    if (strength >= 8) {
        std::cout << name << " 選擇加注" << std::endl;
        return Action::RAISE;
    }
    
    // 中等牌：跟注（如果不貴）
    if (strength >= 5) {
        if (toCall <= chips / 4) {
            std::cout << name << " 選擇跟注" << std::endl;
            return Action::CALL;
        }
    }
    
    // 普通牌：跟注小注，否則棄牌
    if (strength >= 3) {
        if (toCall <= chips / 10) {
            std::cout << name << " 選擇跟注" << std::endl;
            return Action::CALL;
        }
    }
    
    // 爛牌：棄牌
    std::cout << name << " 選擇棄牌" << std::endl;
    return Action::FOLD;
}

int AIPlayer::getRaiseAmount(int currentBet, int minRaise) {
    int strength = getHandStrength();
    int toCall = currentBet - this->currentBet;
    
    // 根據手牌強度決定加注金額
    int raise = toCall + minRaise;
    
    if (strength >= 9) {
        raise = std::min(chips, toCall + minRaise * 3);
    } else if (strength >= 7) {
        raise = std::min(chips, toCall + minRaise * 2);
    } else {
        raise = std::min(chips, toCall + minRaise);
    }
    
    return raise;
}