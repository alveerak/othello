#include "player.hpp"
#include <cstdlib>
#include <vector>

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
/*Move *Player::doMove(Move *opponentsMove, int ms_left) {
    board->doMove(opponentsMove, opp_color);
    if (board->hasMoves(our_color))
    {
      int best_x = 0, best_y = 0, best_score = -1;
      for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++){
          Move *m = new Move(x, y);
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
}*/

//Replacement function minimax style
Move *Player::doMove(Move *opponentsMove, int ms_left) {
    this->board->doMove(opponentsMove, opp_color);
    Move * move = minimax(6);
    this->board->doMove(move, our_color);
    return move;
}

Move *Player::minimax(int depth)
{
  //save all possible moves to vector
  vector <Move *> possible_moves;
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++){
      Move *m = new Move(x, y);
      if (board->checkMove(m, our_color))
      {
        possible_moves.push_back(m);
      }
      else
      {
        delete m;
      }
    }
  }

  if (possible_moves.size() == 0)
  {
    return NULL;
  }
  //recursive call, minimax
  int temp_score;//maximize
  int alpha = -999999; //maximize
  int beta = 999999;//minimize
  int best_x, best_y;
  for (unsigned int i = 0; i < possible_moves.size(); i++)
  {
    //try the move out on the board
    Board *testBoard = this->board->copy();
    Move *test = possible_moves[i];
    testBoard->doMove(test, our_color);
    //save the maximum of the minimum scores from the moves
    temp_score = this->rec_mm(testBoard, depth-1, alpha, beta, opp_color);
    if (alpha <= temp_score)
    {
      alpha = temp_score;
      best_x = test->getX();
      best_y = test->getY();
    }
    delete testBoard;
  }

  // get rid of memory leaks
  for (unsigned int i = 0; i < possible_moves.size(); i++)
  {
    Move *m = possible_moves[i];
    delete m;
  }
  Move *move = new Move(best_x, best_y);
  return move;
}

//recursive calls to minimax
int Player::rec_mm(Board *newboard, int depth, int alpha, int beta, Side side)
{
  if (depth == 0 || newboard->isDone())
  {
    return (newboard->count(our_color)-newboard->count(opp_color));
  }
  //save all possible moves to vector
  vector <Move *> possible_moves;
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++){
      Move *m = new Move(x, y);
      if (newboard->checkMove(m, side))
      {
        possible_moves.push_back(m);
      }
      else
      {
        delete m;
      }
    }
  }

  if (possible_moves.size() == 0)
  {
    return (newboard->count(our_color)-newboard->count(opp_color));
  }

  //recursive call!
  //if our side, maximize our score
  //if their side, anticipate minimizing our score.
  int score;
  if (side == our_color){
    score = -999999;
    int temp_score;
    for (unsigned int i = 0; i < possible_moves.size(); i++)
    {
      //try the move out on the board
      Board *testBoard = newboard->copy();
      Move *test = possible_moves[i];
      testBoard->doMove(test, side);
      //save the maximum of the minimum scores from the moves
      temp_score = this->rec_mm(testBoard, depth - 1, alpha, beta, opp_color);
      if (temp_score > score){
        score = temp_score;
      }
      if (alpha < score){
        alpha = score;
      }
      //quit if waste of time and quit searching in subtree
      if (beta <= alpha)
      {
        break;
      }
      delete testBoard;
    }
    // get rid of memory leaks
    for (unsigned int i = 0; i < possible_moves.size(); i++)
    {
      Move *m = possible_moves[i];
      delete m;
    }
    //return score;
    return alpha;
  }
  else //minimizing
  {
    score = 999999;
    int temp_score;
    for (unsigned int i = 0; i < possible_moves.size(); i++)
    {
      //try the move out on the board
      Board *testBoard = newboard->copy();
      Move *test = possible_moves[i];
      testBoard->doMove(test, side);
      //save the maximum of the minimum scores from the moves
      temp_score = this->rec_mm(testBoard, depth - 1, alpha, beta, our_color);
      if (temp_score < score){
        score = temp_score;
      }
      if (score < beta){
        beta = score;
      }
      //quit searching in subtree if waste of time
      if (beta <= alpha)
      {
        break;
      }
      delete testBoard;
    }
    // get rid of memory leaks
    for (unsigned int i = 0; i < possible_moves.size(); i++)
    {
      Move *m = possible_moves[i];
      delete m;
    }
    //return score;
    return beta;
  }
}
