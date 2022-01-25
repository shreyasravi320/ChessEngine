#include "include/move.h"

int castlingRights[64] = 
{
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

void addMove(MoveList &moves, int move)
{
    moves[moves.size] = move;
    moves.size++;
}

void printMove(int move)
{
    cout << sqToCoords[getStart(move)] << sqToCoords[getTarget(move)] << promToChar[getPromotedPiece(move)];
}

void printMoveList(MoveList &moves)
{
    cout << "move\tpiece    capture    double    enpassant    castling\n\n";
    for (int i = 0; i < moves.size; i++)
    {
        printMove(moves[i]);
        cout << "\t" << pieceToChar[getPiece(moves[i])] << "\t ";
        cout << getCapture(moves[i]) << "\t    ";
        cout << getDoublePush(moves[i]) << "\t      ";
        cout << getEnPassant(moves[i]) << "\t\t   ";
        cout << getCastle(moves[i]) << "\n";
    }

    cout << "\nMoves in list: " << moves.size << "\n";
}

void generateMoves(MoveList &moves)
{
    for (int pc = P; pc <= k; pc++)
    {
        // pawns
        if (turn == WHITE && pc == P)
        {
            Board board = pieces[P];

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
                        addMove(moves, encodeMove(start, target, P, N, 0, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, P, B, 0, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, P, R, 0, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, P, Q, 0, 0, 0, 0));
                    }

                    else
                    {
                        // one square
                        addMove(moves, encodeMove(start, target, P, 0, 0, 0, 0, 0));

                        // two squares
                        if (start >= a2 && start <= h2 && !getBit(occupied[BOTH], target - 8))
                        {
                            addMove(moves, encodeMove(start, target - 8, P, 0, 0, 1, 0, 0));
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
                        addMove(moves, encodeMove(start, target, P, N, 1, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, P, B, 1, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, P, R, 1, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, P, Q, 1, 0, 0, 0));
                    }

                    else
                    {
                        addMove(moves, encodeMove(start, target, P, 0, 1, 0, 0, 0));
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
                        addMove(moves, encodeMove(start, target, P, 0, 1, 0, 1, 0));
                    }
                }

                popBit(board, start);
            }
        }

        else if (turn == BLACK && pc == p)
        {
            Board board = pieces[p];

            while (board)
            {
                int start = lsbIdx(board);
                int target = start + 8;
                
                // Quiet pawn moves (no attacks)
                // check that square is on board
                if (target <= h1 && !getBit(occupied[BOTH], target))
                {
                    // promotion
                    if (start >= a2 && start <= h2)
                    {
                        addMove(moves, encodeMove(start, target, p, n, 0, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, p, b, 0, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, p, r, 0, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, p, q, 0, 0, 0, 0));
                    }

                    else
                    {
                        // one square
                        addMove(moves, encodeMove(start, target, p, 0, 0, 0, 0, 0));

                        // two squares
                        if (start >= a7 && start <= h7 && !getBit(occupied[BOTH], target + 8))
                        {
                            addMove(moves, encodeMove(start, target + 8, p, 0, 0, 1, 0, 0));
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
                        addMove(moves, encodeMove(start, target, p, n, 1, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, p, b, 1, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, p, r, 1, 0, 0, 0));
                        addMove(moves, encodeMove(start, target, p, q, 1, 0, 0, 0));
                    }

                    else
                    {
                        addMove(moves, encodeMove(start, target, p, 0, 1, 0, 0, 0));
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
                        addMove(moves, encodeMove(start, target, p, 0, 1, 0, 1, 0));
                    }
                }

                popBit(board, start);
            }
        }

        // knight attacks
        if ((turn == WHITE) ? pc == N : pc == n)
        {
            Board board = pieces[pc];

            while (board)
            {
                int start = lsbIdx(board);
                Board attacks = knightAttacks[start] & ((turn == WHITE) ? ~occupied[WHITE] : ~occupied[BLACK]);

                while (attacks)
                {
                    int target = lsbIdx(attacks);

                    // attack
                    if (getBit(((turn == WHITE) ? occupied[BLACK] : occupied[WHITE]), target))
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? N : n), 0, 1, 0, 0, 0));
                    }

                    // quiet move
                    else
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? N : n), 0, 0, 0, 0, 0));
                    }

                    popBit(attacks, target);
                }

                popBit(board, start);
            }
        }

        // bishop attacks
        if ((turn == WHITE) ? pc == B : pc == b)
        {
            Board board = pieces[pc];

            while (board)
            {
                int start = lsbIdx(board);
                Board attacks = getBishopAttacks(start, occupied[BOTH]) & ((turn == WHITE) ? ~occupied[WHITE] : ~occupied[BLACK]);

                while (attacks)
                {
                    int target = lsbIdx(attacks);

                    // attack
                    if (getBit(((turn == WHITE) ? occupied[BLACK] : occupied[WHITE]), target))
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? B : b), 0, 1, 0, 0, 0));
                    }

                    // quiet move
                    else
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? B : b), 0, 0, 0, 0, 0));
                    }

                    popBit(attacks, target);
                }

                popBit(board, start);
            }
        }

        // rook attacks
        if ((turn == WHITE) ? pc == R : pc == r)
        {
            Board board = pieces[pc];

            while (board)
            {
                int start = lsbIdx(board);
                Board attacks = getRookAttacks(start, occupied[BOTH]) & ((turn == WHITE) ? ~occupied[WHITE] : ~occupied[BLACK]);

                while (attacks)
                {
                    int target = lsbIdx(attacks);

                    // attack
                    if (getBit(((turn == WHITE) ? occupied[BLACK] : occupied[WHITE]), target))
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? R : r), 0, 1, 0, 0, 0));
                    }

                    // quiet move
                    else
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? R : r), 0, 0, 0, 0, 0));
                    }

                    popBit(attacks, target);
                }

                popBit(board, start);
            }
        }

        // queen attacks
        if ((turn == WHITE) ? pc == Q : pc == q)
        {
            Board board = pieces[pc];

            while (board)
            {
                int start = lsbIdx(board);
                Board attacks = getQueenAttacks(start, occupied[BOTH]) & ((turn == WHITE) ? ~occupied[WHITE] : ~occupied[BLACK]);

                while (attacks)
                {
                    int target = lsbIdx(attacks);

                    // attack
                    if (getBit(((turn == WHITE) ? occupied[BLACK] : occupied[WHITE]), target))
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? Q : q), 0, 1, 0, 0, 0));
                    }

                    // quiet move
                    else
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? Q : q), 0, 0, 0, 0, 0));
                    }

                    popBit(attacks, target);
                }

                popBit(board, start);
            }
        }

        // king attacks
        if ((turn == WHITE) ? pc == K : pc == k)
        {
            Board board = pieces[pc];

            while (board)
            {
                int start = lsbIdx(board);
                Board attacks = kingAttacks[start] & ((turn == WHITE) ? ~occupied[WHITE] : ~occupied[BLACK]);

                while (attacks)
                {
                    int target = lsbIdx(attacks);

                    // attack
                    if (getBit(((turn == WHITE) ? occupied[BLACK] : occupied[WHITE]), target))
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? K : k), 0, 1, 0, 0, 0));
                    }

                    // quiet move
                    else
                    {
                        addMove(moves, encodeMove(start, target, (turn == WHITE ? K : k), 0, 0, 0, 0, 0));
                    }

                    popBit(attacks, target);
                }

                popBit(board, start);
            }
        }

        // castle
        if (turn == WHITE && pc == K)
        {
            if (castle & WK)
            {
                if (!getBit(occupied[BOTH], f1) && !getBit(occupied[BOTH], g1))
                {
                    if (!checkSquareAttacked(BLACK, e1) && !checkSquareAttacked(BLACK, f1))
                    {
                        addMove(moves, encodeMove(e1, g1, K, 0, 0, 0, 0, 1));
                    }
                }
            }

            if (castle & WQ)
            {
                if (!getBit(occupied[BOTH], d1) && !getBit(occupied[BOTH], c1) && !getBit(occupied[BOTH], b1))
                {
                    if (!checkSquareAttacked(BLACK, e1) && !checkSquareAttacked(BLACK, d1))
                    {
                        addMove(moves, encodeMove(e1, c1, K, 0, 0, 0, 0, 1));
                    }
                }
            }
        }

        else if (turn == BLACK && pc == k)
        {
            if (castle & BK)
            {
                if (!getBit(occupied[BOTH], f8) && !getBit(occupied[BOTH], g8))
                {
                    if (!checkSquareAttacked(WHITE, e8) && !checkSquareAttacked(WHITE, f8))
                    {
                        addMove(moves, encodeMove(e8, g8, k, 0, 0, 0, 0, 1));
                    }
                }
            }

            if (castle & BQ)
            {
                if (!getBit(occupied[BOTH], d8) && !getBit(occupied[BOTH], c8) && !getBit(occupied[BOTH], b8))
                {
                    if (!checkSquareAttacked(WHITE, e8) && !checkSquareAttacked(WHITE, d8))
                    {
                        addMove(moves, encodeMove(e8, c8, k, 0, 0, 0, 0, 1));
                    }
                }
            }
        }
    }
}

int makeMove(int move, int moveType)
{
    if (moveType == 0)
    {
        copyBoard();

        int s = getStart(move);
        int t = getTarget(move);
        int pc = getPiece(move);
        int pro = getPromotedPiece(move);
        int c = getCapture(move);
        int dp = getDoublePush(move);
        int eps = getEnPassant(move);
        int cstl = getCastle(move);

        popBit(pieces[pc], s);
        setBit(pieces[pc], t);

        // captures
        if (c)
        {
            for (int i = (turn == WHITE ? p : P); i <= (turn == WHITE ? k : K); i++)
            {
                if (getBit(pieces[i], t))
                {
                    popBit(pieces[i], t);
                    break;
                }
            }
        }

        // pawn promotion
        if (pro)
        {
            popBit(pieces[(turn == WHITE ? P : p)], t);
            setBit(pieces[pro], t);
        }

        // en passant
        if (eps)
        {
            popBit(pieces[(turn == WHITE ? p : P)], (turn == WHITE ? (t + 8) : (t - 8)));
        }
        enPass = -1;

        // double push pawn
        if (dp)
        {
            enPass = (turn == WHITE ? (t + 8) : (t - 8));
        }

        // castle
        if (cstl)
        {
            switch (t)
            {
                // WK
                case g1:
                    popBit(pieces[R], h1);
                    setBit(pieces[R], f1);
                    break;
                
                // WQ
                case c1:
                    popBit(pieces[R], a1);
                    setBit(pieces[R], d1);
                    break;
                
                // BK
                case g8:
                    popBit(pieces[r], h8);
                    setBit(pieces[r], f8);
                    break;

                // BQ
                case c8:
                    popBit(pieces[r], a8);
                    setBit(pieces[r], d8);
                    break;
            }
        }

        castle &= castlingRights[s];
        castle &= castlingRights[t];

        memset(occupied, 0ULL, 3 * sizeof(Board));

        for (int i = P; i <= K; i++)
        {
            occupied[WHITE] |= pieces[i];
            occupied[BLACK] |= pieces[i + p];
        }

        occupied[BOTH] = (occupied[WHITE] | occupied[BLACK]);
        turn ^= 1;

        // make sure move is valid
        if (checkSquareAttacked(turn, (turn == WHITE ? lsbIdx(pieces[k]) : lsbIdx(pieces[K]))))
        {
            undo();
            return 0;
        }

        return 1;
    }
    else
    {
        if (getCapture(move))
        {
            return makeMove(move, 0);
        }
        
        return 0;
    }
}