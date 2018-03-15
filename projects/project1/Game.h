//
//  Game.h
//  CS32 - PROJECT 1
//
//  Created by Stefanie Shidoosh on 4/6/17.
//  Copyright © 2017 Stefanie Shidoosh. All rights reserved.
//

#ifndef Game_h
#define Game_h
#include <iostream>
class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    std::string takePlayerTurn();
};




#endif /* Game_hpp */
