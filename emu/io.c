#include "emu8008.h"
#include <stdio.h>

static u_int8_t x, y;

static void async_write(u_int8_t v)
{
    putc(v, stderr);
}

static struct device {
    u_int8_t n;
    u_int8_t state;
    void (*write)(u_int8_t v);
} devices[] = {
    { 0322, 0, async_write }
};

static struct device * current_device;

static void INP(u_int8_t p)
{
    pc++;
    cycles(16);
}

static void OUT(u_int8_t p)
{
    switch(p) {
        case 8:
            for(current_device = devices; current_device->n != regs[0]; current_device++);
            break;
        case 9:
            // ???????
            break;
        case 10:
            // ???????
            break;
        case 11:
            current_device->write(regs[0]);
            break;
        case 12:
            // ???????
            break;
        case 13:
            x = regs[0];
            break;
        case 14:
            y = regs[0];
            break;
        case 15:
            // ???????
            break;
        case 31:
            fprintf(stderr, "state=%x\tflags=%x%x%x%x\n", state, flags[3], flags[2], flags[1], flags[0]);
            fprintf(stderr, "sp\tpc\t\n%x\t%x\n", sp, pc);
            fprintf(stderr, "A\tB\tC\tD\tE\tH\tL\n");
            for(int i = 0; i < 7; i++) fprintf(stderr, "%02x\t", regs[i]);
            fprintf(stderr, "\n");
            getc(stdin);
            break;
    }
    pc++;
    cycles(12);
}

void INP0()     { INP(0); }
void INP1()     { INP(1); }
void INP2()     { INP(2); }
void INP3()     { INP(3); }
void INP4()     { INP(4); }
void INP5()     { INP(5); }
void INP6()     { INP(6); }
void INP7()     { INP(7); }

void OUT8()     { OUT(8); }
void OUT9()     { OUT(9); }
void OUT10()    { OUT(10); }
void OUT11()    { OUT(11); }
void OUT12()    { OUT(12); }
void OUT13()    { OUT(13); }
void OUT14()    { OUT(14); }
void OUT15()    { OUT(15); }

void OUT16()    { OUT(16); }
void OUT17()    { OUT(17); }
void OUT18()    { OUT(18); }
void OUT19()    { OUT(19); }
void OUT20()    { OUT(20); }
void OUT21()    { OUT(21); }
void OUT22()    { OUT(22); }
void OUT23()    { OUT(23); }

void OUT24()    { OUT(24); }
void OUT25()    { OUT(25); }
void OUT26()    { OUT(26); }
void OUT27()    { OUT(27); }
void OUT28()    { OUT(28); }
void OUT29()    { OUT(29); }
void OUT30()    { OUT(30); }
void OUT31()    { OUT(31); }
