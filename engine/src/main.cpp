#include "include/search.h"

int main()
{
    initAttacks();

    parseFen(START);
    engine(6);

    return 0;
}