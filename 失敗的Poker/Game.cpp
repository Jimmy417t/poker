#include "Game.h"
#include <iostream>
#include <algorithm>

Game::Game() : pot(0), currentBet(0), dealerIndex(0), smallBlind(10), bigBlind(20) {}

Game::~Game() {
    for (Player* p : players) {
        delete p;
    }
}

void Game::addPlayer(Player* p) {
    players.push_back(p);
}

void Game::dealHoleCards() {
    for (Player* p : players) {
        p->receiveCard(deck.dealCard());
        p->receiveCard(deck.dealCard());
    }
}

void Game::dealFlop() {
    deck.dealCard(); // 燒一張
    communityCards.push_back(deck.dealCard());
    communityCards.push_back(deck.dealCard());
    communityCards.push_back(deck.dealCard());
}

void Game::dealTurn() {
    deck.dealCard(); // 燒一張
    communityCards.push_back(deck.dealCard());
}

void Game::dealRiver() {
    deck.dealCard(); // 燒一張
    communityCards.push_back(deck.dealCard());
}

void Game::printCommunityCards() const {
    std::cout << "公牌：";
    for (const auto& card : communityCards) {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
}

void Game::printPlayerStatus() const {
    std::cout << "\n=== 目前狀態 ===" << std::endl;
    for (Player* p : players) {
        std::cout << p->getName() << " 籌碼：" << p->getChips() 
                  << " 目前下注：" << p->getCurrentBet() << std::endl;
    }
    std::cout << "底池：" << pot << std::endl;
    if (!communityCards.empty()) {
        printCommunityCards();
    }
    std::cout << std::endl;
}

void Game::bettingRound() {
    std::vector<Player*> activePlayers;
    for (Player* p : players) {
        if (p->getChips() > 0) {
            activePlayers.push_back(p);
        }
    }
    
    bool bettingDone = false;
    int lastRaiseIndex = -1;
    int currentPlayerIndex = 0;
    
    while (!bettingDone) {
        bool allCalled = true;
        int targetBet = currentBet;
        
        for (Player* p : activePlayers) {
            if (p->getCurrentBet() < targetBet) {
                allCalled = false;
                break;
            }
        }
        
        if (allCalled && currentPlayerIndex == lastRaiseIndex) {
            bettingDone = true;
            break;
        }
        
        Player* current = activePlayers[currentPlayerIndex % activePlayers.size()];
        
        int toCall = currentBet - current->getCurrentBet();
        
        if (toCall > 0 || currentBet == 0) {
            Action action = current->takeTurn(currentBet, bigBlind);
            
            switch (action) {
                case Action::FOLD:
                    // 標記為棄牌，從活躍玩家移除
                    activePlayers.erase(activePlayers.begin() + (currentPlayerIndex % activePlayers.size()));
                    if (currentPlayerIndex >= (int)activePlayers.size()) {
                        currentPlayerIndex = 0;
                    }
                    continue;
                    
                case Action::CALL:
                    current->deductChips(toCall);
                    current->setCurrentBet(currentBet);
                    pot += toCall;
                    break;
                    
                case Action::RAISE: {
                    int raiseAmount = current->getRaiseAmount(currentBet, bigBlind);
                    if (raiseAmount < currentBet + bigBlind) {
                        raiseAmount = currentBet + bigBlind;
                    }
                    if (raiseAmount > current->getChips() + current->getCurrentBet()) {
                        raiseAmount = current->getChips() + current->getCurrentBet();
                    }
                    int additional = raiseAmount - current->getCurrentBet();
                    current->deductChips(additional);
                    current->setCurrentBet(raiseAmount);
                    pot += additional;
                    currentBet = raiseAmount;
                    lastRaiseIndex = currentPlayerIndex % activePlayers.size();
                    break;
                }
            }
        }
        
        currentPlayerIndex++;
        
        if (activePlayers.size() == 1) {
            awardPot(activePlayers[0]);
            return;
        }
    }
}

void Game::collectBets() {
    for (Player* p : players) {
        pot += p->getCurrentBet();
        p->setCurrentBet(0);
    }
    currentBet = 0;
}

void Game::resetRound() {
    for (Player* p : players) {
        p->clearHand();
        p->setCurrentBet(0);
    }
    communityCards.clear();
    deck.reset();
    deck.shuffle();
}

HandRankResult Game::getBestHand(Player* player) {
    std::vector<Card> allCards = player->getHand();
    allCards.insert(allCards.end(), communityCards.begin(), communityCards.end());
    
    // 從 7 張牌中選出最佳 5 張組合
    HandRankResult best;
    best.type = HandType::HIGH_CARD;
    
    std::vector<int> indices = {0,1,2,3,4,5,6};
    // 簡化：直接用 7 張牌判斷（實務上需要選最佳 5 張）
    return HandRank::evaluate(allCards);
}

void Game::awardPot(Player* winner) {
    winner->addChips(pot);
    std::cout << winner->getName() << " 贏得底池 " << pot << " 籌碼！" << std::endl;
    pot = 0;
}

void Game::showdown() {
    std::cout << "\n=== 攤牌 ===" << std::endl;
    printCommunityCards();
    
    Player* winner = nullptr;
    HandRankResult bestHand;
    
    for (Player* p : players) {
        if (p->getChips() > 0) {
            std::cout << p->getName() << " 的手牌：";
            p->showHand();
            HandRankResult hand = getBestHand(p);
            std::cout << "牌型：";
            switch (hand.type) {
                case HandType::HIGH_CARD: std::cout << "高牌"; break;
                case HandType::ONE_PAIR: std::cout << "一對"; break;
                case HandType::TWO_PAIR: std::cout << "兩對"; break;
                case HandType::THREE_OF_A_KIND: std::cout << "三條"; break;
                case HandType::STRAIGHT: std::cout << "順子"; break;
                case HandType::FLUSH: std::cout << "同花"; break;
                case HandType::FULL_HOUSE: std::cout << "葫蘆"; break;
                case HandType::FOUR_OF_A_KIND: std::cout << "鐵支"; break;
                case HandType::STRAIGHT_FLUSH: std::cout << "同花順"; break;
                case HandType::ROYAL_FLUSH: std::cout << "皇家同花順"; break;
            }
            std::cout << std::endl;
            
            if (winner == nullptr || HandRank::compare(hand, bestHand) > 0) {
                winner = p;
                bestHand = hand;
            }
        }
    }
    
    std::cout << "\n獲勝者：" << winner->getName() << std::endl;
    awardPot(winner);
}

void Game::start() {
    std::cout << "=== 德州撲克遊戲開始 ===" << std::endl;
    std::cout << "小盲注：" << smallBlind << "，大盲注：" << bigBlind << std::endl;
}

void Game::run() {
    start();
    
    int round = 1;
    while (true) {
        std::cout << "\n========== 第 " << round << " 局 ==========" << std::endl;
        
        // 檢查遊戲結束
        int activeCount = 0;
        Player* lastPlayer = nullptr;
        for (Player* p : players) {
            if (p->getChips() > 0) {
                activeCount++;
                lastPlayer = p;
            }
        }
        
        if (activeCount == 1) {
            std::cout << lastPlayer->getName() << " 贏得整場遊戲！" << std::endl;
            break;
        }
        
        resetRound();
        
        // 發牌
        dealHoleCards();
        
        // 下注輪：Pre-flop
        std::cout << "\n=== Pre-flop ===" << std::endl;
        currentBet = bigBlind;
        bettingRound();
        collectBets();
        
        // Flop
        dealFlop();
        std::cout << "\n=== Flop ===" << std::endl;
        printCommunityCards();
        currentBet = 0;
        bettingRound();
        collectBets();
        
        // Turn
        dealTurn();
        std::cout << "\n=== Turn ===" << std::endl;
        printCommunityCards();
        currentBet = 0;
        bettingRound();
        collectBets();
        
        // River
        dealRiver();
        std::cout << "\n=== River ===" << std::endl;
        printCommunityCards();
        currentBet = 0;
        bettingRound();
        collectBets();
        
        // 攤牌
        showdown();
        
        round++;
        
        char c;
        std::cout << "按 Enter 繼續下一局...";
        std::cin.ignore();
        std::cin.get();
    }
}