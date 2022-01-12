#ifndef PERFT_H
#define PERFT_H

#include "move.h"

extern long nodes;

void perftDriver(int depth);
void perftTest(int depth);

#endif