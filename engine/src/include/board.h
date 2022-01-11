#ifndef LOGIC_H
#define LOGIC_H

#include <string>
using std::string;

#include "utils.h"
#include "attack.h"

// FEN debug positions
#define EMPTY "8/8/8/8/8/8/8/8 w - -"
#define START "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define POS_1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define POS_2 "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define POS_3 "rn1q1Q2/pp5p/3pk2B/1P3Q2/3pP3/2NP3P/P1P5/R3KB1R b KQ - 0 13"

// Create a board for each type of piece
extern Board pieces[12];

// Create a board for each side { white, black, both }
extern Board occupied[3];
extern int turn;
extern int enPass;
extern int castle;

void printBoard();
void parseFen(string FEN);

bool checkSquareAttacked(int turn, int sq);
void printAttackedSquares(int turn);

#endif