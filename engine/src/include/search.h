#ifndef SEARCH_H
#define SEARCH_H

#include "move.h"

extern int mirrored[64];

// Points based on material difference
extern int material[12];

// Points based on location of pieces
extern int pawnPos[64];
extern int knightPos[64];
extern int bishopPos[64];
extern int rookPos[64];
extern int kingPos[64];

int evaluate();
int quiescence(int alpha, int beta);
int negaMax(int depth, int alpha, int beta);
void engine(int depth);

#endif