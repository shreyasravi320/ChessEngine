#ifndef BOARD_H
#define BOARD_H

#define Board unsigned long long

#define set(b, sq) (b |= (1ULL << sq))
#define get(b, sq) (b & (1ULL << sq))
#define pop(b, sq) (b &= ~(1ULL << sq))

enum
{
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

enum
{
    WHITE,
    BLACK,
    BOTH
};

// Masks for A file, H file, AB files, and GH files
const Board NOT_A_FILE = 18374403900871474942ULL;
const Board NOT_H_FILE = 9187201950435737471ULL;
const Board NOT_AB_FILE = 18229723555195321596ULL;
const Board NOT_GH_FILE = 4557430888798830399ULL;

// Precomputed attack tables [turn][squares]
extern Board pawnAttacks[2][64];
extern Board knightAttacks[64];
extern Board kingAttacks[64];

Board maskPawnAttacks(int turn, int sq);
Board maskKnightAttacks(int sq);
Board maskKingAttacks(int sq);

void initLeaperAttacks();

void printBoard(Board b);

#endif