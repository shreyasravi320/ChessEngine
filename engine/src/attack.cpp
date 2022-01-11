#include "include/attack.h"

/*
 * Precomputed attack tables for all pieces
 */
Board pawnAttacks[2][64];
Board knightAttacks[64];
Board kingAttacks[64];

Board bishopMasks[64];
Board rookMasks[64];

Board bishopAttacks[64][512];
Board rookAttacks[64][4096];

Board maskPawnAttacks(int turn, int sq)
{
    Board attacks = 0ULL;

    Board b = 0ULL;
    setBit(b, sq);
    
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
    setBit(b, sq);
    
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
    setBit(b, sq);

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

Board maskBishopAttacks(int sq)
{
    Board attacks = 0ULL;
    
    int tr = sq / 8;
    int tf = sq % 8;
    
    for (int r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
    }

    for (int r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
    }

    for (int r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
    }

    for (int r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
    }
    
    return attacks;
}

Board maskRookAttacks(int sq)
{
    Board attacks = 0ULL;
    
    int tr = sq / 8;
    int tf = sq % 8;
    
    for (int r = tr + 1; r <= 6; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
    }

    for (int r = tr - 1; r >= 1; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
    }

    for (int f = tf - 1; f >= 1; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
    }

    for (int f = tf + 1; f <= 6; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
    }
    
    return attacks;
}

Board generateBishopAttacks(int sq, Board blockers)
{
    Board attacks = 0ULL;

    int tr = sq / 8;
    int tf = sq % 8;
    
    for (int r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & blockers)
        {
            break;
        }
    }

    for (int r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & blockers)
        {
            break;
        }
    }

    for (int r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & blockers)
        {
            break;
        }
    }

    for (int r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & blockers)
        {
            break;
        }
    }

    return attacks;
}

Board generateRookAttacks(int sq, Board blockers)
{
    Board attacks = 0ULL;
    
    int tr = sq / 8;
    int tf = sq % 8;
    
    for (int r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & blockers)
        {
            break;
        }
    }

    for (int r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & blockers)
        {
            break;
        }
    }

    for (int f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & blockers)
        {
            break;
        }
    }

    for (int f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & blockers)
        {
            break;
        }
    }
    
    return attacks;
}

Board setOccupied(int idx, Board bitCount, Board attackMask)
{
    Board occupied = 0ULL;

    for (int i = 0; i < bitCount; i++)
    {
        int sq = lsbIdx(attackMask);
        popBit(attackMask, sq);

        if (idx & (1 << i))
        {
            occupied |= (1ULL << sq);
        }
    }

    return occupied;
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

void initSliderAttacks()
{
    for (int sq = 0; sq < 64; sq++)
    {
        bishopMasks[sq] = maskBishopAttacks(sq);
        rookMasks[sq] = maskRookAttacks(sq);

        // loop over bishop and rook
        for (int p = 0; p < 2; p++)
        {
            Board attackMask = p ? rookMasks[sq] : bishopMasks[sq];
            int relBitCount = count(attackMask);
            int occIdxs = (1 << relBitCount);
            
            for (int i = 0; i < occIdxs; i++)
            {
                if (p)
                {
                    Board occupied = setOccupied(i, relBitCount, attackMask);
                    int magicIdx = (occupied * rookMagicNumbers[sq]) >> (64 - rookRelBits[sq]);
                    rookAttacks[sq][magicIdx] = generateRookAttacks(sq, occupied);
                }
                else
                {
                    Board occupied = setOccupied(i, relBitCount, attackMask);
                    int magicIdx = (occupied * bishopMagicNumbers[sq]) >> (64 - bishopRelBits[sq]);
                    bishopAttacks[sq][magicIdx] = generateBishopAttacks(sq, occupied);
                }
            }
        }
    }
}

void initAttacks()
{
    initLeaperAttacks();
    initSliderAttacks();
}

Board getBishopAttacks(int sq, Board occupied)
{
    occupied &= bishopMasks[sq];
    occupied *= bishopMagicNumbers[sq];
    occupied >>= (64 - bishopRelBits[sq]);
    
    return bishopAttacks[sq][occupied];
}

Board getRookAttacks(int sq, Board occupied)
{
    occupied &= rookMasks[sq];
    occupied *= rookMagicNumbers[sq];
    occupied >>= (64 - rookRelBits[sq]);

    return rookAttacks[sq][occupied];
}

Board getQueenAttacks(int sq, Board occupied)
{
    return (getBishopAttacks(sq, occupied) | getRookAttacks(sq, occupied));
}

void printBoard(Board b)
{
    cout << "\n";
    for (int r = 0; r < 8; r++)
    {
        cout << "  " << 8 - r << " "; 
        for (int f = 0; f < 8; f++)
        {
            int sq = r * 8 + f;
            cout << " " << (getBit(b, sq) ? 1 : 0);
        }
        cout << "\n";
    }
    cout << "     a b c d e f g h\n";
}