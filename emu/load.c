#include "emu8008.h"

static void load_ri(u_int8_t r)
{
    pc++;
    regs[r] = *(code + pc);
    pc++;
    cycles(16);
}

static void load_mi()
{
    pc++;
    *m_reg() = *(code + pc);
    pc++;
    cycles(18);
}

static void load_rr(u_int8_t d, u_int8_t s)
{
    regs[d] = regs[s];
    pc++;
    cycles(10);
}

static void load_rm(u_int8_t r)
{
    regs[r] = *m_reg();
    pc++;
    cycles(16);
}

static void load_mr(u_int8_t r)
{
    *m_reg() = regs[r];
    pc++;
    cycles(14);
}

void LAI()  { load_ri(0); }
void LBI()  { load_ri(1); }
void LCI()  { load_ri(2); }
void LDI()  { load_ri(3); }
void LEI()  { load_ri(4); }
void LHI()  { load_ri(5); }
void LLI()  { load_ri(6); }
void LMI()  { load_mi(); }

void LAB()  { load_rr(0, 1); }
void LAC()  { load_rr(0, 2); }
void LAD()  { load_rr(0, 3); }
void LAE()  { load_rr(0, 4); }
void LAH()  { load_rr(0, 5); }
void LAL()  { load_rr(0, 6); }
void LAM()  { load_rm(0); }

void LBA()  { load_rr(1, 0); }
void LBB()  { load_rr(1, 1); }
void LBC()  { load_rr(1, 2); }
void LBD()  { load_rr(1, 3); }
void LBE()  { load_rr(1, 4); }
void LBH()  { load_rr(1, 5); }
void LBL()  { load_rr(1, 6); }
void LBM()  { load_rm(1); }

void LCA()  { load_rr(2, 0); }
void LCB()  { load_rr(2, 1); }
void LCC()  { load_rr(2, 2); }
void LCD()  { load_rr(2, 3); }
void LCE()  { load_rr(2, 4); }
void LCH()  { load_rr(2, 5); }
void LCL()  { load_rr(2, 6); }
void LCM()  { load_rm(2); }

void LDA()  { load_rr(3, 0); }
void LDB()  { load_rr(3, 1); }
void LDC()  { load_rr(3, 2); }
void LDD()  { load_rr(3, 3); }
void LDE()  { load_rr(3, 4); }
void LDH()  { load_rr(3, 5); }
void LDL()  { load_rr(3, 6); }
void LDM()  { load_rm(3); }

void LEA()  { load_rr(4, 0); }
void LEB()  { load_rr(4, 1); }
void LEC()  { load_rr(4, 2); }
void LED()  { load_rr(4, 3); }
void LEE()  { load_rr(4, 4); }
void LEH()  { load_rr(4, 5); }
void LEL()  { load_rr(4, 6); }
void LEM()  { load_rm(4); }

void LHA()  { load_rr(5, 0); }
void LHB()  { load_rr(5, 1); }
void LHC()  { load_rr(5, 2); }
void LHD()  { load_rr(5, 3); }
void LHE()  { load_rr(5, 4); }
void LHH()  { load_rr(5, 5); }
void LHL()  { load_rr(5, 6); }
void LHM()  { load_rm(5); }

void LLA()  { load_rr(6, 0); }
void LLB()  { load_rr(6, 1); }
void LLC()  { load_rr(6, 2); }
void LLD()  { load_rr(6, 3); }
void LLE()  { load_rr(6, 4); }
void LLH()  { load_rr(6, 5); }
void LLL()  { load_rr(6, 6); }
void LLM()  { load_rm(6); }

void LMA()  { load_mr(0); }
void LMB()  { load_mr(1); }
void LMC()  { load_mr(2); }
void LMD()  { load_mr(3); }
void LME()  { load_mr(4); }
void LMH()  { load_mr(5); }
void LML()  { load_mr(6); }
