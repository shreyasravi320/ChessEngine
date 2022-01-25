#ifndef SEARCH_H
#define SEARCH_H

#include "move.h"

const int MAX_PLY = 64;

extern int mirrored[64];

// Points based on material difference
extern int material[12];

// Points based on location of pieces
extern int pawnPos[64];
extern int knightPos[64];
extern int bishopPos[64];
extern int rookPos[64];
extern int kingPos[64];

/*
 * Most Valuable Victim - Least Valuable Attacker (MVVLVA)
 * 
 * We can use this table to sort which attacks are better
 * For example, if the opponent has a defended queen, then pawn takes queen is 
 * much better than queen takes queen (trade)
 *
 *       (V/A)   Pawn    Knight  Bishop  Rook    Queen   King
 *        Pawn   105     205     305     405     505     605
 *      Knight   104     204     304     404     504     604
 *      Bishop   103     203     303     403     503     603
 *        Rook   102     202     302     402     502     602
 *       Queen   101     201     301     401     501     601
 *        King   100     200     300     400     500     600
 */

extern int MVV_LVA[12][12];

// Store 2 killer moves
extern int killerMoves[2][MAX_PLY];

// Store history moves for each piece on the squares
extern int historyMoves[12][64];

/*
 * Triangle Principal Variation Table
 * 
 *      0   1   2   3   4   5
 *  0   m1  m2  m3  m4  m5  m6
 *  1   --  m2  m3  m4  m5  m6
 *  2   --  --  m3  m4  m5  m6
 *  3   --  --  --  m4  m5  m6
 *  4   --  --  --  --  m5  m6
 *  5   --  --  --  --  --  m6
 * 
 */

extern int PV_LENGTH[MAX_PLY];
extern int PV_TABLE[MAX_PLY][MAX_PLY];

int evaluate();
int scoreMove(int move);
void sortMoves(MoveList &moves);
void printMoveScores(MoveList &moves);

int quiescence(int alpha, int beta);
int negaScout(int depth, int alpha, int beta);
void engine(int depth);

#endif