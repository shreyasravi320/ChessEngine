#include "include/search.h"

int ply = 0;
int bestMove = 0;

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

int quiescence(int alpha, int beta)
{
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

int negaMax(int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return quiescence(alpha, beta);
    }

    int check = checkSquareAttacked(turn ^ 1, (turn == WHITE ? lsbIdx(pieces[K]) : lsbIdx(pieces[k])));
    int legal = 0;
    int best = 0;
    int old = alpha;

    MoveList moves;
    generateMoves(moves);

    for (int i = 0; i < moves.size; i++)
    {
        int score = 0;
        copyBoard();
        ply++;

        if (makeMove(moves[i], 0))
        {
            legal++;
            score = -negaMax(depth - 1, -beta, -alpha);
            ply--;
            undo();

            if (score >= beta)
            {
                return beta;
            }

            if (score > alpha)
            {
                alpha = score;

                if (ply == 0)
                {
                    best = moves[i];
                }
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

    if (old != alpha)
    {
        bestMove = best;
    }

    return alpha;
}

void engine(int depth)
{
    auto start = high_resolution_clock::now();
    int score = negaMax(depth, -50000, 50000);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    if (bestMove)
    {
        cout << "Best move: ";
        printMove(bestMove);
        cout << "Score: " << score << "\n";
        cout << "Depth: " << depth << "\n";
        cout << "Time taken: " << duration.count() << " milliseconds\n\n";
    }
}