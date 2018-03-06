#include "player.hpp"
#include <cstdlib>

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    this -> board = new Board;
    this->our_color = side;
    if (side == BLACK)
    {
      this->opp_color = WHITE;
    }
    else
    {
      this->opp_color = BLACK;
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {
  delete board;
}

/* Computes score for move */
int Player::calcScore(int x, int y) {
  //return 1;
  return weights[y][x];
}
/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int ms_left) {
    board->doMove(opponentsMove, opp_color);
    if (board->hasMoves(our_color))
    {
      int best_x = 0, best_y = 0, best_score = -1;
      for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++){
          Move *m = new Move(x, y);
          /*if (board->checkMove(m, our_color)){
            board->doMove(m, our_color);
            return m;
          }*/
          if (board->checkMove(m, our_color) &&
              calcScore(x, y) > best_score)
          {
            best_score = calcScore(x, y);
            best_x = x;
            best_y = y;
          }
          delete m;
        }
      }
      Move *best_move = new Move(best_x, best_y);
      board->doMove(best_move, our_color);
      return best_move;
    }
    return nullptr;
}
