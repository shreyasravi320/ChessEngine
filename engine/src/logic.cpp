#include "include/logic.h"
#include <iostream>
using std::cout;

Board pieces[12];
Board occupied[3];
int turn = -1;
int enPass = -1;
int castle = -1;

void printBoard()
{
    for (int r = 0; r < 8; r++)
    {
        cout << "  " << 8 - r << " ";
        for (int f = 0; f < 8; f++)
        {
            int sq = r * 8 + f;
            int piece = -1;

            for (int pc = P; pc <= k; pc++)
            {
                if (getBit(pieces[pc], sq))
                {
                    piece = pc;
                    break;
                }
            }

            cout << " " << (piece == -1 ? '.' : pieceToChar[piece]);
        }
        cout << "\n";
    }
    cout << "     a b c d e f g h\n\n";
    cout << "Turn: " << (turn == WHITE ? "White" : "Black") << "\n";
    cout << "En Pass: " << (enPass == -1 ? "--" : sqToCoords[enPass]) << "\n";

    cout << "Castle Perms: ";
    cout << (castle & WK ? "K" : "-");
    cout << (castle & WQ ? "Q" : "-");
    cout << (castle & BK ? "k" : "-");
    cout << (castle & BQ ? "q" : "-");
    cout << "\n\n";
}

void parseFen(string FEN)
{
    memset(pieces, 0ULL, 12 * sizeof(Board));
    memset(occupied, 0ULL, 3 * sizeof(Board));
    turn = WHITE;
    enPass = -1;
    castle = 0;

    int i = 0;
    for (int r = 0; r < 8; r++)
    {
        for (int f = 0; f < 8; f++)
        {
            int sq = r * 8 + f;
            if ((FEN[i] >= 'A' && FEN[i] <= 'Z') || (FEN[i] >= 'a' && FEN[i] <= 'z'))
            {
                int piece = charToPiece[FEN[i]];
                setBit(pieces[piece], sq);
                i++;
            }

            if (FEN[i] >= '0' && FEN[i] <= '8')
            {
                int offset = FEN[i] - '0';
                int piece = -1;

                for (int pc = P; pc <= k; pc++)
                {
                    if (getBit(pieces[pc], sq))
                    {
                        piece = pc;
                    }
                }

                if (piece == -1)
                {
                    f--;
                }

                f += offset;
                i++;
            }

            if (FEN[i] == '/')
            {
                i++;
            }
        }
    }

    i++;
    FEN[i] == 'w' ? turn = WHITE : turn = BLACK;
    
    i += 2;
    while (FEN[i] != ' ')
    {
        switch (FEN[i])
        {
            case 'K': 
                castle |= WK;
                break;

            case 'Q': 
                castle |= WQ;
                break;

            case 'k': 
                castle |= BK;
                break;

            case 'q': 
                castle |= BQ;
                break;
            
            case '-':
                break;
        }

        i++;
    }

    i++;
    if (FEN[i] != '-')
    {
        int file = FEN[i] - 'a';
        int rank = 8 - (FEN[i + 1] - '0');
        enPass = rank * 8 + file;
    }
    else
    {
        enPass = -1;
    }

    for (int pc = P; pc <= K; pc++)
    {
        occupied[WHITE] |= pieces[pc];
        occupied[BLACK] |= pieces[pc + p];
    }

    occupied[BOTH] |= (occupied[WHITE] | occupied[BLACK]);
}

bool checkSquareAttacked(int turn, int sq)
{
    if (turn == WHITE && (pawnAttacks[BLACK][sq] & pieces[P]))
    {
        return true;
    }

    if (turn == BLACK && (pawnAttacks[WHITE][sq] & pieces[p]))
    {
        return true;
    }

    if (knightAttacks[sq] & (turn == WHITE ? pieces[N] : pieces[n]))
    {
        return true;
    }

    if (getBishopAttacks(sq, occupied[BOTH]) & (turn == WHITE ? pieces[B] : pieces[b]))
    {
        return true;
    }

    if (getRookAttacks(sq, occupied[BOTH]) & (turn == WHITE ? pieces[R] : pieces[r]))
    {
        return true;
    }

    if (getQueenAttacks(sq, occupied[BOTH]) & (turn == WHITE ? pieces[Q] : pieces[q]))
    {
        return true;
    }

    if (kingAttacks[sq] & (turn == WHITE ? pieces[K] : pieces[k]))
    {
        return true;
    }

    return false;
}

void printAttackedSquares(int turn)
{
    for (int r = 0; r < 8; r++)
    {
        cout << "  " << 8 - r << " ";
        for (int f = 0; f < 8; f++)
        {
            int sq = r * 8 + f;
            cout << " " << (checkSquareAttacked(turn, sq) ? 1 : 0);
        }
        cout << "\n";
    }
    cout << "     a b c d e f g h\n\n";
}

void generateMoves()
{
    for (int pc = P; pc <= k; pc++)
    {
        Board board = pieces[pc];

        // white pawns
        if (pc == P)
        {
            if (turn == WHITE)
            {
                while (board)
                {
                    int start = lsbIdx(board);
                    int target = start - 8;
                    
                    // Quiet pawn moves (no attacks)
                    // check that square is on board
                    if (target >= a8 && !getBit(occupied[BOTH], target))
                    {
                        // promotion
                        if (start >= a7 && start <= h7)
                        {
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "N\n";
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "B\n";
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "R\n";
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "Q\n";
                        }

                        else
                        {
                            // one square
                            cout << "pawn push: " << sqToCoords[start] << sqToCoords[target] << "\n";

                            if (start >= a2 && start <= h2)
                            {
                                cout << "pawn double push: " << sqToCoords[start] << sqToCoords[target - 8] << "\n";
                            }
                        }
                    }

                    // Attacks
                    Board attacks = pawnAttacks[turn][start] & occupied[BLACK];
                    while (attacks)
                    {
                        target = lsbIdx(attacks);
                        
                        // promotion
                        if (start >= a7 && start <= h7)
                        {
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "N\n";
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "B\n";
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "R\n";
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "Q\n";
                        }

                        else
                        {
                            cout << "pawn capture: " << sqToCoords[start] << sqToCoords[target] << "\n";
                        }

                        popBit(attacks, target);
                    }

                    // En Passant
                    if (enPass != -1)
                    {
                        Board enPassAttacks = pawnAttacks[turn][start] & (1ULL << enPass);

                        if (enPassAttacks)
                        {
                            target = lsbIdx(enPassAttacks);
                            cout << "pawn en passant capture: " << sqToCoords[start] << sqToCoords[target] << "\n";
                        }
                    }

                    popBit(board, start);
                }
            }

            else
            {
                while (board)
                {
                    int start = lsbIdx(board);
                    int target = start - 8;
                    
                    // Quiet pawn moves (no attacks)
                    // check that square is on board
                    if (target <= h1 && !getBit(occupied[BOTH], target))
                    {
                        // promotion
                        if (start >= a2 && start <= h2)
                        {
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "n\n";
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "b\n";
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "r\n";
                            cout << "pawn promotion: " << sqToCoords[start] << sqToCoords[target] << "q\n";
                        }

                        else
                        {
                            // one square
                            cout << "pawn push: " << sqToCoords[start] << sqToCoords[target] << "\n";

                            if (start >= a7 && start <= h7)
                            {
                                cout << "pawn double push: " << sqToCoords[start] << sqToCoords[target - 8] << "\n";
                            }
                        }
                    }

                    // Attacks
                    Board attacks = pawnAttacks[turn][start] & occupied[WHITE];
                    while (attacks)
                    {
                        target = lsbIdx(attacks);
                        
                        // promotion
                        if (start >= a2 && start <= h2)
                        {
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "n\n";
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "b\n";
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "r\n";
                            cout << "pawn promotion capture: " << sqToCoords[start] << sqToCoords[target] << "q\n";
                        }

                        else
                        {
                            cout << "pawn capture: " << sqToCoords[start] << sqToCoords[target] << "\n";
                        }

                        popBit(attacks, target);
                    }

                    // En Passant
                    if (enPass != -1)
                    {
                        Board enPassAttacks = pawnAttacks[turn][start] & (1ULL << enPass);

                        if (enPassAttacks)
                        {
                            target = lsbIdx(enPassAttacks);
                            cout << "pawn en passant capture: " << sqToCoords[start] << sqToCoords[target] << "\n";
                        }
                    }

                    popBit(board, start);
                }
            }
        }
    }
}