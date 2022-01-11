#ifndef BOARD_H
#define BOARD_H

#include "utils.h"

// Relevant occupied bits for every square on the board
extern Board bishopRelBits[64];
extern Board rookRelBits[64];

// Precomputed attack tables
// [turn][squares]
extern Board pawnAttacks[2][64];

// [squares]
extern Board knightAttacks[64];
extern Board kingAttacks[64];

// [squares][occupied]
extern Board bishopAttacks[64][512];
extern Board rookAttacks[64][4096];

Board maskPawnAttacks(int turn, int sq);
Board maskKnightAttacks(int sq);
Board maskKingAttacks(int sq);

Board maskBishopAttacks(int sq);
Board maskRookAttacks(int sq);
Board generateBishopAttacks(int sq, Board blockers);
Board generateRookAttacks(int sq, Board blockers);

Board setOccupied(int idx, Board bitCount, Board attackMask);

void initLeaperAttacks();
void initSliderAttacks();
void initAttacks();

Board getBishopAttacks(int sq, Board occupied);
Board getRookAttacks(int sq, Board occupied);
Board getQueenAttacks(int sq, Board occupied);

void printBoard(Board b);

#endif