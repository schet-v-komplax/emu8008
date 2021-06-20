#include "emu8008.h"
#include <stdbool.h>

static void jmp_cond(int cond, bool state)
{
    if(flags[cond] == state) JMP();
    else {
        pc += 3;
        cycles(18);
    }
}

static void cal_cond(int cond, bool state)
{
    if(flags[cond] == state) CAL();
    else {
        pc += 3;
        cycles(18);
    }
}

static void ret_cond(int cond, bool state)
{
    if(flags[cond] == state) RET();
    else {
        pc++;
        cycles(18);
    }
}

static void do_RST(u_int16_t r)
{
    sp--;
    *(stack + sp) = pc + 1;
    pc = r << 3; 
    cycles(10);
}

void JMP()
{ 
    pc = *(u_int16_t *)(code + pc + 1); 
    cycles(22); 
}

void CAL() 
{
    sp--;
    *(stack + sp) = pc + 3;
    pc = *(u_int16_t *)(code + pc + 1); 
    cycles(22);
}

void RET()
{
    pc = *(stack + sp);
    sp++;
    cycles(22);
}

void JFC()  { jmp_cond(0, false); }
void JFZ()  { jmp_cond(1, false); }
void JFS()  { jmp_cond(2, false); }
void JFP()  { jmp_cond(3, false); }
void JTC()  { jmp_cond(0, true); }
void JTZ()  { jmp_cond(1, true); }
void JTS()  { jmp_cond(2, true); }
void JTP()  { jmp_cond(3, true); }

void CFC()  { cal_cond(0, false); }
void CFZ()  { cal_cond(1, false); }
void CFS()  { cal_cond(2, false); }
void CFP()  { cal_cond(3, false); }
void CTC()  { cal_cond(0, true); }
void CTZ()  { cal_cond(1, true); }
void CTS()  { cal_cond(2, true); }
void CTP()  { cal_cond(3, true); }

void RFC()  { ret_cond(0, false); }
void RFZ()  { ret_cond(1, false); }
void RFS()  { ret_cond(2, false); }
void RFP()  { ret_cond(3, false); }
void RTC()  { ret_cond(0, true); }
void RTZ()  { ret_cond(1, true); }
void RTS()  { ret_cond(2, true); }
void RTP()  { ret_cond(3, true); }

void RST0() { do_RST(0); }
void RST1() { do_RST(1); }
void RST2() { do_RST(2); }
void RST3() { do_RST(3); }
void RST4() { do_RST(4); }
void RST5() { do_RST(5); }
void RST6() { do_RST(6); }
void RST7() { do_RST(7); }
