#ifndef UTILS_H
#define UTILS_H

#include <iostream>
using std::cout;

#include <string>
using std::string;

#include <chrono>
using namespace std::chrono;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::microseconds;
using std::chrono::milliseconds;

#include <cstring>

#define Board unsigned long long

// Helpful board data
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

extern string sqToCoords[64];
extern char pieceToChar[12];
extern int charToPiece[];
extern char promToChar[];

// Turn data
enum
{
    WHITE,
    BLACK,
    BOTH
};

// Piece data (uppercase represents white, lowercase represents black)
enum
{
    P, N, B, R, Q, K, 
    p, n, b, r, q, k
};

/*
 * Castling data (4 bits necessary)
 * 0001     1  white castles king side
 * 0010     2  white castles queen side
 * 0100     4  black castles king side
 * 1000     8  black castles queen side
 * 
 * 1011 indicates that white can castle king and queen side, and black can castle queen side
 */
enum
{
    WK = 1,
    WQ = 2,
    BK = 4,
    BQ = 8
};

#define setBit(b, sq) ((b) |= (1ULL << (sq)))
#define getBit(b, sq) ((b) & (1ULL << (sq)))
#define popBit(b, sq) ((b) &= ~(1ULL << (sq)))

extern unsigned int state;

int count(Board b);
int log2(Board b);
int lsbIdx(Board b);

unsigned int rand32();
unsigned long long rand64();
unsigned long long magicNumber();

// Masks for A file, H file, AB files, and GH files
const Board NOT_A_FILE = 18374403900871474942ULL;
const Board NOT_H_FILE = 9187201950435737471ULL;
const Board NOT_AB_FILE = 18229723555195321596ULL;
const Board NOT_GH_FILE = 4557430888798830399ULL;

// Magic numbers for every square on the board
extern Board bishopMagicNumbers[64];
extern Board rookMagicNumbers[64];

// Attack masks for sliding pieces
extern Board bishopMasks[64];
extern Board rookMasks[64];

#endif