#include "emu8008.h"

static void set_flags(int16_t x)
{
    flags[0] = (x & 0xff00) != 0;
    flags[1] = x == 0;
    flags[2] = x < 0;
    flags[3] = (~__builtin_parity(x)) & 1;
}

static u_int8_t add(u_int8_t s) { return regs[0] += s; }
static u_int8_t adc(u_int8_t s) { return regs[0] += s + flags[0]; }
static u_int8_t sub(u_int8_t s) { return regs[0] -= s; }
static u_int8_t sbb(u_int8_t s) { return regs[0] -= s + flags[0]; }
static u_int8_t and(u_int8_t s) { return regs[0] &= s; }
static u_int8_t xor(u_int8_t s) { return regs[0] ^= s; }
static u_int8_t or(u_int8_t s)  { return regs[0] |= s; }
static u_int8_t cmp(u_int8_t s) { return regs[0] - s; }

static void inc(u_int8_t r)
{
    regs[r]++;
    pc++;
    set_flags(regs[r]);
    cycles(10);
}

static void dec(u_int8_t r)
{
    regs[r]--;
    pc++;
    set_flags(regs[r]);
    cycles(10);
}

static void math_r(u_int8_t r, u_int8_t (* func)(u_int8_t s))
{
    set_flags(func(regs[r]));
    pc++;
    cycles(10);
}

static void math_m(u_int8_t (* func)(u_int8_t s))
{
    set_flags(func(*m_reg()));
    pc++;
    cycles(16);
}

void math_i(u_int8_t (* func)(u_int8_t s))
{
    pc++;
    set_flags(func(*(code + pc)));
    pc++;
    cycles(16);
}

void ADA()  { math_r(0, add); }
void ADB()  { math_r(1, add); }
void ADC()  { math_r(2, add); }
void ADD()  { math_r(3, add); }
void ADE()  { math_r(4, add); }
void ADH()  { math_r(5, add); }
void ADL()  { math_r(6, add); }
void ADM()  { math_m(add); }
void ADI()  { math_i(add); }

void ACA()  { math_r(0, adc); }
void ACB()  { math_r(1, adc); }
void ACC()  { math_r(2, adc); }
void ACD()  { math_r(3, adc); }
void ACE()  { math_r(4, adc); }
void ACH()  { math_r(5, adc); }
void ACL()  { math_r(6, adc); }
void ACM()  { math_m(adc); }
void ACI()  { math_i(adc); }

void SUA()  { math_r(0, sub); }
void SUB()  { math_r(1, sub); }
void SUC()  { math_r(2, sub); }
void SUD()  { math_r(3, sub); }
void SUE()  { math_r(4, sub); }
void SUH()  { math_r(5, sub); }
void SUL()  { math_r(6, sub); }
void SUM()  { math_m(sub); }
void SUI()  { math_i(sub); }

void SBA()  { math_r(0, sbb); }
void SBB()  { math_r(1, sbb); }
void SBC()  { math_r(2, sbb); }
void SBD()  { math_r(3, sbb); }
void SBE()  { math_r(4, sbb); }
void SBH()  { math_r(5, sbb); }
void SBL()  { math_r(6, sbb); }
void SBM()  { math_m(sbb); }
void SBI()  { math_i(sbb); }

void NDA()  { math_r(0, and); }
void NDB()  { math_r(1, and); }
void NDC()  { math_r(2, and); }
void NDD()  { math_r(3, and); }
void NDE()  { math_r(4, and); }
void NDH()  { math_r(5, and); }
void NDL()  { math_r(6, and); }
void NDM()  { math_m(and); }
void NDI()  { math_i(and); }

void XRA()  { math_r(0, xor); }
void XRB()  { math_r(1, xor); }
void XRC()  { math_r(2, xor); }
void XRD()  { math_r(3, xor); }
void XRE()  { math_r(4, xor); }
void XRH()  { math_r(5, xor); }
void XRL()  { math_r(6, xor); }
void XRM()  { math_m(xor); }
void XRI()  { math_i(xor); }

void ORA()  { math_r(0, or); }
void ORB()  { math_r(1, or); }
void ORC()  { math_r(2, or); }
void ORD()  { math_r(3, or); }
void ORE()  { math_r(4, or); }
void ORH()  { math_r(5, or); }
void ORL()  { math_r(6, or); }
void ORM()  { math_m(or); }
void ORI()  { math_i(or); }

void CPA()  { math_r(0, cmp); }
void CPB()  { math_r(1, cmp); }
void CPC()  { math_r(2, cmp); }
void CPD()  { math_r(3, cmp); }
void CPE()  { math_r(4, cmp); }
void CPH()  { math_r(5, cmp); }
void CPL()  { math_r(6, cmp); }
void CPM()  { math_m(cmp); }
void CPI()  { math_i(cmp); }

void INB()  { inc(1); }
void INC()  { inc(2); }
void IND()  { inc(3); }
void INE()  { inc(4); }
void INH()  { inc(5); }
void INL()  { inc(6); }

void DCB()  { dec(1); }
void DCC()  { dec(2); }
void DCD()  { dec(3); }
void DCE()  { dec(4); }
void DCH()  { dec(5); }
void DCL()  { dec(6); }

void RLC() 
{
    flags[0] = regs[0] >> 7;
    regs[0] = (regs[0] << 1) | flags[0];
    pc++;
    cycles(10);
}

void RAL()
{
    regs[0] <<= 1;
    regs[0] |= flags[0];
    flags[0] = (regs[0] & 0xff00) != 0;
    pc++;
    cycles(10);
}

void RRC()
{
    flags[0] = regs[0] & 1;
    regs[0] = (regs[0] >> 1) | (flags[0] << 7);
    pc++;
    cycles(10);
}

void RAR() 
{
    u_int8_t tmp = regs[0];
    regs[0] >>= 1;
    regs[0] |= flags[0] << 7;
    flags[0] = tmp & 1;
    pc++;
    cycles(10);
}
