#include "Player.h"
#include <iostream>

Player::Player(std::string n, int startingChips) 
    : name(n), chips(startingChips), currentBet(0) {}

void Player::receiveCard(Card c) {
    hand.push_back(c);
}

void Player::clearHand() {
    hand.clear();
    currentBet = 0;
}

void Player::showHand() const {
    for (const auto& card : hand) {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
}

std::vector<Card> Player::getHand() const {
    return hand;
}

int Player::getChips() const {
    return chips;
}

void Player::deductChips(int amount) {
    chips -= amount;
}

void Player::addChips(int amount) {
    chips += amount;
}

void Player::setCurrentBet(int bet) {
    currentBet = bet;
}

int Player::getCurrentBet() const {
    return currentBet;
}

std::string Player::getName() const {
    return name;
}

Action Player::takeTurn(int currentBet, int minRaise) {
    std::cout << name << " 的回合，籌碼：" << chips << std::endl;
    std::cout << "手牌：";
    showHand();
    std::cout << "目前下注：" << currentBet << "，你需要跟注：" << (currentBet - this->currentBet) << std::endl;
    std::cout << "1) 棄牌  2) 跟注  3) 加注" << std::endl;
    std::cout << "請選擇：";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: return Action::FOLD;
        case 2: return Action::CALL;
        case 3: return Action::RAISE;
        default: return Action::FOLD;
    }
}

int Player::getRaiseAmount(int currentBet, int minRaise) {
    int toCall = currentBet - this->currentBet;
    std::cout << "目前需跟注：" << toCall << "，最小加注：" << minRaise << std::endl;
    std::cout << "請輸入加注金額（總額）：";
    int amount;
    std::cin >> amount;
    return amount;
}

bool Player::isActive() const {
    return chips > 0;
}