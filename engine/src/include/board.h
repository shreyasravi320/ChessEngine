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
#define POS_2 "8/p7/1P6/8/K1k3p1/6P1/7P/8 b - - 0 1"
#define POS_3 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define POS_4 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
#define POS_5 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"

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