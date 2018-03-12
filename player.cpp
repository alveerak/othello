#include "player.hpp"
#include <cstdlib>
#include <vector>

/**
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
    int depth = 7;
    if (testingMinimax){
        depth = 2;
    }
    Move * move = minimax(depth);
    this->board->doMove(move, our_color);
    return move;
}

/**
 * @brief Executes the minimax algorithm of depth.
 * 
 * @param depth, max depth of minimax
 */
Move *Player::minimax(int depth)
{
  // Save all possible moves to vector possible_moves
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
  // Quit if no moves available
  if (possible_moves.size() == 0)
  {
    return NULL;
  }
  // Recursively call minimax
  int temp_score;       //stores best score so far
  int alpha = -999999; 	//maximum score found so far regardless of what
						//opponent plays
  int beta = 999999;	//minimum opponent score found so far regardless
						//of what player plays
  int best_x, best_y;
  Move test = Move(0,0);
  for (unsigned int i = 0; i < possible_moves.size(); i++)
  {
    //try the move out on the board
    Board *testBoard = this->board->copy();
    test = Move(possible_moves[i]->x, possible_moves[i]->y);
    testBoard->doMove(&test, our_color);
    //save the maximum of the minimum scores from the moves
    if (testingMinimax){
            temp_score = this->rec_mm(testBoard, depth-1, alpha, beta, opp_color);
    }
    else{
        temp_score = calcScore(test.getX(), test.getY())+this->rec_mm(testBoard, depth-1, alpha, beta, opp_color);
    }
    if (alpha <= temp_score)
    {
      alpha = temp_score;
      best_x = test.getX();
      best_y = test.getY();
    }
    delete testBoard;
  }

  // Clean up memory leaks
  for (unsigned int i = 0; i < possible_moves.size(); i++)
  {
    Move *m = possible_moves[i];
    delete m;
  }
  Move *move = new Move(best_x, best_y);
  return move;
}

/**
 * @brief Recursive version of the minimax algorithm.
 * 
 * @param depth, depth of minimax
 * @param alpha, maximum player score found so far
 * @param beta, minimum opponent score so far
 * @param side, WHITE or BLACK depending on the player's color
 */
int Player::rec_mm(Board *newboard, int depth, int alpha, int beta, Side side)
{
  // Base case, depth of 0 or no moves left
  if (depth == 0 || newboard->isDone())
  {
    return (newboard->count(our_color)-newboard->count(opp_color));
  }
  // Aave all possible moves to vector possible_moves
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
      if (testingMinimax){
          temp_score = this->rec_mm(testBoard, depth - 1, alpha, beta, opp_color);
      }else{
          temp_score = calcScore(test->getX(), test->getY()) +
                       this->rec_mm(testBoard, depth - 1, alpha, beta, opp_color);
      }
      if (temp_score > score){
        score = temp_score;
      }
      //if (alpha < score){
      //  alpha = score;
      //}
      // Quit if opponent is guaranteed a higher score
      if (beta <= score)
      {
        break;
      }
      delete testBoard;
    }
    // Clean up memory
    for (unsigned int i = 0; i < possible_moves.size(); i++)
    {
      Move *m = possible_moves[i];
      delete m;
    }
    return score;
    //return alpha;
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
      if (testingMinimax){
          temp_score = this->rec_mm(testBoard, depth - 1, alpha, beta, our_color);
      }else{
          temp_score = calcScore(test->getX(), test->getY()) +
                       this->rec_mm(testBoard, depth - 1, alpha, beta, our_color);
      }
      if (temp_score < score){
        score = temp_score;
      }
      //if (score < beta){
      //  beta = score;
      //}
      //quit searching in subtree if waste of time
      if (score <= alpha)
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
    return score;
    //return beta;
  }
}
