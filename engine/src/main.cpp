#include "include/move.h"

int main()
{
    initAttacks();
    parseFen(POS_1);

    MoveList moves;
    moves.size = 0;
    generateMoves(moves);

    auto start = high_resolution_clock::now();
    printMoveList(moves);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << duration.count() << "\n";

    return 0;
}