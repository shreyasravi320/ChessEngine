#include "include/perft.h"

long _nodes = 0;

void perftDriver(int depth)
{
    if (depth == 0)
    {
        _nodes++;
        return;
    }

    MoveList moves;
    generateMoves(moves);

    for (int i = 0; i < moves.size; i++)
    {
        copyBoard();

        if (makeMove(moves[i], 0))
        {
            perftDriver(depth - 1);
            undo();
        }
    }
}

void perftTest(int depth)
{
    cout << "Performance test\n\n";
    _nodes = 0;

    printBoard();
    MoveList moves;
    generateMoves(moves);
    auto start = high_resolution_clock::now();

    for (int i = 0; i < moves.size; i++)
    {
        copyBoard();

        if (makeMove(moves[i], 0))
        {
            long __nodes = _nodes;
            perftDriver(depth - 1);
            long diff = _nodes - __nodes;
            
            cout << "Move: " << sqToCoords[getStart(moves[i])] << sqToCoords[getTarget(moves[i])] 
                << promToChar[getPromotedPiece(moves[i])] << "\tNodes: " << diff << "\n";
            
            undo();
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "\nDepth: " << depth << "\n";
    cout << "Nodes: " << _nodes << "\n";
    cout << "Time: " << duration.count() << " microseconds\n";
}