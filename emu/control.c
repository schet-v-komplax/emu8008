#include "emu8008.h"

void HLT()
{
    state = STOP;
    pc++;
    cycles(8);
}

void NOP()
{
    pc++;
    cycles(10);
}
