#include "Game.h"
#include "Player.h"
#include "AIPlayer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(nullptr));
    
    Game game;
    
    // 加入玩家
    game.addPlayer(new Player("玩家", 1000));
    game.addPlayer(new AIPlayer("電腦1", 1000));
    game.addPlayer(new AIPlayer("電腦2", 1000));
    
    game.run();
    
    return 0;
}