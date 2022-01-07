#include <iostream>
using std::cout;

#include "include/board.h"

Board pawnAttacks[2][64];
Board knightAttacks[64];
Board kingAttacks[64];

Board maskPawnAttacks(int turn, int sq)
{
    Board attacks = 0ULL;

    Board b = 0ULL;
    set(b, sq);
    
    // white pawns
    if (turn == WHITE)
    {
        if ((b >> 7) & NOT_A_FILE)
        {
            attacks |= (b >> 7);
        }

        if ((b >> 9) & NOT_H_FILE)
        {
            attacks |= (b >> 9);
        }
    }
    
    // black pawns
    else
    {
        if ((b << 7) & NOT_H_FILE)
        {
            attacks |= (b << 7);
        }

        if ((b << 9) & NOT_A_FILE)
        {
            attacks |= (b << 9); 
        }   
    }

    return attacks;
}

Board maskKnightAttacks(int sq)
{
    Board attacks = 0ULL;

    Board b = 0ULL;
    set(b, sq);
    
    if ((b >> 15) & NOT_A_FILE)
    {
        attacks |= (b >> 15);
    }
    if ((b >> 17) & NOT_H_FILE)
    {
        attacks |= (b >> 17);
    }
    if ((b >> 6) & NOT_AB_FILE)
    {
        attacks |= (b >> 6);
    }
    if ((b >> 10) & NOT_GH_FILE)
    {
        attacks |= (b >> 10);
    }

    if ((b << 17) & NOT_A_FILE)
    {
        attacks |= (b << 17);
    } 
    if ((b << 15) & NOT_H_FILE)
    {
        attacks |= (b << 15);
    }
    if ((b << 10) & NOT_AB_FILE)
    {
        attacks |= (b << 10);
    }
    if ((b << 6) & NOT_GH_FILE)
    {
        attacks |= (b << 6);
    }

    return attacks;
}

Board maskKingAttacks(int sq)
{
    Board attacks = 0ULL;

    Board b = 0ULL;
    set(b, sq);

    if (b >> 8)
    {
        attacks |= (b >> 8);
    }
    if ((b >> 9) & NOT_H_FILE)
    {
        attacks |= (b >> 9);
    }
    if ((b >> 7) & NOT_A_FILE)
    {
        attacks |= (b >> 7);
    }
    if ((b >> 1) & NOT_H_FILE)
    {
        attacks |= (b >> 1);
    }

    if (b << 8)
    {
        attacks |= (b << 8);
    }
    if ((b << 9) & NOT_A_FILE)
    {
        attacks |= (b << 9);
    }
    if ((b << 7) & NOT_H_FILE)
    {
        attacks |= (b << 7);
    }
    if ((b << 1) & NOT_A_FILE)
    {
        attacks |= (b << 1);
    }

    return attacks;
}

void initLeaperAttacks()
{
    for (int sq = 0; sq < 64; sq++)
    {
        pawnAttacks[WHITE][sq] = maskPawnAttacks(WHITE, sq);
        pawnAttacks[BLACK][sq] = maskPawnAttacks(BLACK, sq);

        knightAttacks[sq] = maskKnightAttacks(sq);

        kingAttacks[sq] = maskKingAttacks(sq);
    }
}

void printBoard(Board b)
{
    cout << "\n";
    for (int rank = 0; rank < 8; rank++)
    {
        cout << "  " << 8 - rank << " "; 
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            cout << " " << (get(b, sq) ? 1 : 0);
        }
        cout << "\n";
    }
    cout << "     a b c d e f g h\n";
}