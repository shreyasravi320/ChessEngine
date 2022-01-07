#include "include/board.h"

void printBoard(Board b)
{
    printf("\n");

    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;
            
            if (!file)
                printf("  %d ", 8 - rank);
            
            printf(" %d", get(b, sq) ? 1 : 0);
            
        }
        printf("\n");
    }
    
    printf("\n     a b c d e f g h\n\n");
}