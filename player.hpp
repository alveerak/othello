#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

//position weights from riscos.com Figure 9.3 on Othello weightings
static int weights [8][8] = {{7, 2, 5, 4, 4, 5, 2, 7},
                 {2, 1, 3, 3, 3, 3, 1, 2},
                 {5, 3, 6, 5, 5, 6, 3, 5},
                 {4, 3, 5, 6, 6, 5, 3, 4},
                 {4, 3, 5, 6, 6, 5, 3, 4},
                 {5, 3, 6, 5, 5, 6, 3, 5},
                 {2, 1, 3, 3, 3, 3, 1, 2},
                 {7, 2, 5, 4, 4, 5, 2, 7}};
class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    int calcScore(int x, int y);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Move *minimax(int depth);
    //int min(Board *board, int depth, int temp_a, int temp_b);
    //int max(Board *board, int depth, int temp_a, int temp_b);
    int rec_mm(Board *board, int depth, int alpha, int beta, Side side);


private:
    Side our_color;
    Side opp_color;
    Board *board;
};

#endif
