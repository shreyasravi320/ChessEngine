#ifndef MOVE_H
#define MOVE_H

#include "board.h"

/*
 * Encoding a move in a 24-bit integer
 *
 * 0000 0000 0000 0000 0011 1111    0x3f        start square   
 * 0000 0000 0000 1111 1100 0000    0xfc0       target square   
 * 0000 0000 1111 0000 0000 0000    0xf000      piece           
 * 0000 1111 0000 0000 0000 0000    0xf0000     promoted piece  
 * 0001 0000 0000 0000 0000 0000    0x100000    capture flag    
 * 0010 0000 0000 0000 0000 0000    0x200000    double push flag
 * 0100 0000 0000 0000 0000 0000    0x400000    enpassant flag  
 * 1000 0000 0000 0000 0000 0000    0x800000    castling flag   
 */

#define encodeMove(s, t, pc, pro, cap, dp, ep, cstl) \
    ((s) | ((t) << 6) | ((pc) << 12) | ((pro) << 16) | \
         ((cap) << 20) | ((dp) << 21) | ((ep) << 22) | ((cstl) << 23)) 

#define getStart(m)         ((m & 0x3f))
#define getTarget(m)        ((m & 0xfc0) >> 6)
#define getPiece(m)         ((m & 0xf000) >> 12)
#define getPromotedPiece(m) ((m & 0xf0000) >> 16)
#define getCapture(m)       ((m & 0x100000) >> 20)
#define getDoublePush(m)    ((m & 0x200000) >> 21)
#define getEnPassant(m)     ((m & 0x400000) >> 22)
#define getCastle(m)        ((m & 0x800000) >> 23)

struct MoveList
{
    int moves[256];
    int size;

    int &operator[](int i)
    {
        return moves[i];
    }
};

void addMove(MoveList &moves, int move);
void printMove(int move);
void printMoveList(MoveList &moves);

void generateMoves(MoveList &moves);

#define copyBoard() \
    Board _pieces[12], _occupied[3]; \
    int _turn, _enPass, _castle; \
    memcpy(_pieces, pieces, 12 * sizeof(Board)); \
    memcpy(_occupied, occupied, 3 * sizeof(Board)); \
    _turn = turn, _enPass = enPass, _castle = castle;

#define undo() \
    memcpy(pieces, _pieces, 12 * sizeof(Board)); \
    memcpy(occupied, _occupied, 3 * sizeof(Board)); \
    turn = _turn, enPass = _enPass, castle = _castle;

/*
 * Castling rights based on target square
 *
 * no movement:             1111 & 1111  =  1111    15
 * 
 * white king moved:        1111 & 1100  =  1100    12
 * white king rook moved:   1111 & 1110  =  1110    14
 * white queen rook moved:  1111 & 1101  =  1101    13
 
 * black king moved:        1111 & 0011  =  1011    3
 * black king rook moved:   1111 & 1011  =  1011    11
 * black queen rook moved:  1111 & 0111  =  0111    7
*/

extern int castlingRights[64];

// distinguish between quiet move (0) and capture/attack (1) for quiescence search
int makeMove(int move, int moveType);

#endif