#include "include/search.h"

int ply = 0;
int nodes = 0;
clock_t START_TIME;

int killerMoves[2][MAX_PLY];
int historyMoves[12][64];

int PV_LENGTH[MAX_PLY];
int PV_TABLE[MAX_PLY][MAX_PLY];
bool scorePV = false;
bool followPV = false;

int mirrored[64] = 
{
    a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

int material[12] = 
{
    100, 300, 350, 500, 1000, 10000,
    -100, -300, -350, -500, -1000, -10000
};

int pawnPos[64] = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

int knightPos[64] = 
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

int bishopPos[64] = 
{
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,  10,  10,   0,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,  10,   0,   0,   0,   0,  10,   0,
    0,  30,   0,   0,   0,   0,  30,   0,
    0,   0, -10,   0,   0, -10,   0,   0
};

int rookPos[64] = 
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0
};

int kingPos[64] =
{
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   5,   5,   5,   5,   0,   0,
    0,   5,   5,  10,  10,   5,   5,   0,
    0,   5,  10,  20,  20,  10,   5,   0,
    0,   5,  10,  20,  20,  10,   5,   0,
    0,   0,   5,  10,  10,   5,   0,   0,
    0,   5,   5,  -5,  -5,   0,   5,   0,
    0,   0,   5,   0, -15,   0,  10,   0
};

int MVV_LVA[12][12] =
{
    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
};

int evaluate()
{
    uint64_t score = 0;
    for (int i = P; i <= k; i++)
    {
        Board brd = pieces[i];

        while(brd)
        {
            int sq = lsbIdx(brd);
            score += material[i];

            switch (i)
            {
                case P:
                    score += pawnPos[sq];
                    break;
                case N:
                    score += knightPos[sq];
                    break;
                case B:
                    score += bishopPos[sq];
                    break;
                case R:
                    score += rookPos[sq];
                    break;
                case K:
                    score += kingPos[sq];
                    break;

                case p:
                    score -= pawnPos[mirrored[sq]];
                    break;
                case n:
                    score -= knightPos[mirrored[sq]];
                    break;
                case b:
                    score -= bishopPos[mirrored[sq]];
                    break;
                case r:
                    score -= rookPos[mirrored[sq]];
                    break;
                case k:
                    score -= kingPos[mirrored[sq]];
                    break;
                
                default:
                    break;
            }

            popBit(brd, sq);
        }
    }

    return (turn == WHITE ? score : -score);
}

int scoreMove(int move)
{
    if (scorePV)
    {
        if (PV_TABLE[0][ply] == move)
        {
            scorePV = false;
            return 20000;
        }
    }

    if (getCapture(move))
    {
        int t = 0;
        for (int i = (turn == BLACK ? P : p); i <= (turn == BLACK ? K : k); i++)
        {
            if (getBit(pieces[i], getTarget(move)))
            {
                return MVV_LVA[getPiece(move)][i];
            }
        }
    }
    else
    {
        if (killerMoves[0][ply] == move)
        {
            return 9000;
        }
        else if (killerMoves[1][ply] == move)
        {
            return 8000;
        }
        else
        {
            return historyMoves[getPiece(move)][getTarget(move)];
        }
    }

    return 0;
}

void enablePVScoring(MoveList &moves)
{
    followPV = false;
    for (int i = 0; i < moves.size; i++)
    {
        if (PV_TABLE[0][ply] == moves[i])
        {
            scorePV = true;
            followPV = true;
            break;
        }
    }
}

void sortMoves(MoveList &moves)
{
    int moveScores[moves.size];
    for (int i = 0; i < moves.size; i++)
    {
        moveScores[i] = scoreMove(moves[i]);
    }
    quickerSort(moveScores, moves.moves, 0, moves.size - 1);
}

void printMoveScores(MoveList &moves)
{
    for (int i = 0; i < moves.size; i++)
    {
        cout << "Move: ";
        printMove(moves[i]);
        cout << "\tScore: " << scoreMove(moves[i]) << "\n";
    }
}

int quiescence(int alpha, int beta)
{
    nodes++;
    int eval = evaluate();
    if (eval >= beta)
    {
        return beta;
    }

    if (eval > alpha)
    {
        alpha = eval;
    }

    MoveList moves;
    generateMoves(moves);
    sortMoves(moves);

    for (int i = 0; i < moves.size; i++)
    {
        int score = 0;
        copyBoard();
        ply++;

        if (makeMove(moves[i], 1))
        {
            score = -quiescence(-beta, -alpha);
            ply--;
            undo();

            if (score >= beta)
            {
                return beta;
            }

            if (score > alpha)
            {
                alpha = score;
            }
        }
        else
        {
            ply--;
        }
    }

    return alpha;
}

int negaScout(int depth, int alpha, int beta)
{
    PV_LENGTH[ply] = ply;

    if (depth == 0)
    {
        return quiescence(alpha, beta);
    }

    nodes++;
    int check = checkSquareAttacked(turn ^ 1, (turn == WHITE ? lsbIdx(pieces[K]) : lsbIdx(pieces[k])));
    if (check)
    {
        depth++;
    }

    int legal = 0;

    MoveList moves;
    generateMoves(moves);

    if (followPV)
    {
        enablePVScoring(moves);
    }

    sortMoves(moves);

    for (int i = 0; i < moves.size; i++)
    {
        int score = 0;
        copyBoard();
        ply++;

        if (makeMove(moves[i], 0))
        {
            legal++;

            // once moves are sorted by PV, the first move will always be the
            // one discovered by the previous search iff no better move exists
            if (i == 0)
            {
                score = -negaScout(depth - 1, -beta, -alpha);
            }

            else
            {
                score = -negaScout(depth - 1, -alpha - 1, -alpha);

                // on fail high, research
                if (score > alpha && score < beta)
                {
                    score = -negaScout(depth - 1, -beta, -score);
                }
            }

            ply--;
            undo();

            if (score >= beta)
            {
                if (!getCapture(moves[i]))
                {
                    killerMoves[1][ply] = killerMoves[0][ply];
                    killerMoves[0][ply] = moves[i];
                }

                return beta;
            }

            if (score > alpha)
            {
                if (!getCapture(moves[i]))
                {
                    historyMoves[getPiece(moves[i])][getTarget(moves[i])] += depth;
                }
                
                alpha = score;
                PV_TABLE[ply][ply] = moves[i];
                memcpy(&PV_TABLE[ply][ply + 1], &PV_TABLE[ply + 1][ply + 1], (PV_LENGTH[ply + 1] - (ply + 1)) * sizeof(int));
                PV_LENGTH[ply] = PV_LENGTH[ply + 1];
            }
        }
        else
        {
            ply--;
        }
    }

    if (legal == 0)
    {
        if (check)
        {
            return -49000 + ply;
        }
        else
        {
            return 0;
        }
    }

    return alpha;
}

void engine(int depth)
{
    int score = 0;
    nodes = 0;
    followPV = false;
    scorePV = false;
    memset(killerMoves, 0, 2 * MAX_PLY * sizeof(int));
    memset(historyMoves, 0, 12 * 64 * sizeof(int));
    memset(PV_TABLE, 0, MAX_PLY * MAX_PLY * sizeof(int));
    memset(PV_LENGTH, 0, MAX_PLY * sizeof(int));

    START_TIME = clock();
    auto start = high_resolution_clock::now();

    for (int i = 1; i <= depth; i++)
    {
        score = negaScout(i, -50000, 50000);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    makeMove(PV_TABLE[0][0], 0);


    printMove(PV_TABLE[0][0]);
    cout << "\n";

    for (int i = 0; i < PV_LENGTH[0]; i++)
    {
        printMove(PV_TABLE[0][i]);
        cout << " ";
    }
    cout << "\n";

    cout << score << "\n";
    cout << duration.count() / 1e3 << "\n";
    cout << nodes << "\n";
}