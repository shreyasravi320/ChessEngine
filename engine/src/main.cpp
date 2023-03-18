#include "include/search.h"

int main()
{
    initAttacks();

    string fen;
    getline(std::cin, fen);
    parseFen(fen);

    engine(6);
    cout << getFEN() << "\n";

    return 0;
}