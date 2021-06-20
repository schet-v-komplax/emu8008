#pragma once

#include <sys/types.h>


extern u_int16_t pc;
extern u_int16_t sp;

extern u_int8_t code[0x4000];
extern u_int16_t stack[8];

extern u_int8_t flags[4];
extern u_int16_t regs[7];

enum state
{
    RUN,
    STOP
};

extern enum state state;

void cycles(int i);
u_int8_t * m_reg();

void HLT();
void NOP();

void INP0();
void INP1();
void INP2();
void INP3();
void INP4();
void INP5();
void INP6();
void INP7();

void OUT8();
void OUT9();
void OUT10();
void OUT11();
void OUT12();
void OUT13();
void OUT14();
void OUT15();

void OUT16();
void OUT17();
void OUT18();
void OUT19();
void OUT20();
void OUT21();
void OUT22();
void OUT23();

void OUT24();
void OUT25();
void OUT26();
void OUT27();
void OUT28();
void OUT29();
void OUT30();
void OUT31();

void JMP();
void JFC();
void JFZ();
void JFS();
void JFP();
void JTC();
void JTZ();
void JTS();
void JTP();

void CAL();
void CFC();
void CFZ();
void CFS();
void CFP();
void CTC();
void CTZ();
void CTS();
void CTP();

void RET();
void RFC();
void RFZ();
void RFS();
void RFP();
void RTC();
void RTZ();
void RTS();
void RTP();

void RST0();
void RST1();
void RST2();
void RST3();
void RST4();
void RST5();
void RST6();
void RST7();

void LAI();
void LBI();
void LCI();
void LDI();
void LEI();
void LHI();
void LLI();
void LMI();

void LAB();
void LAC();
void LAD();
void LAE();
void LAH();
void LAL();
void LAM();

void LBA();
void LBB();
void LBC();
void LBD();
void LBE();
void LBH();
void LBL();
void LBM();

void LCA();
void LCB();
void LCC();
void LCD();
void LCE();
void LCH();
void LCL();
void LCM();

void LDA();
void LDB();
void LDC();
void LDD();
void LDE();
void LDH();
void LDL();
void LDM();

void LEA();
void LEB();
void LEC();
void LED();
void LEE();
void LEH();
void LEL();
void LEM();

void LHA();
void LHB();
void LHC();
void LHD();
void LHE();
void LHH();
void LHL();
void LHM();

void LLA();
void LLB();
void LLC();
void LLD();
void LLE();
void LLH();
void LLL();
void LLM();

void LMA();
void LMB();
void LMC();
void LMD();
void LME();
void LMH();
void LML();

void ADA();
void ADB();
void ADC();
void ADD();
void ADE();
void ADH();
void ADL();
void ADM();
void ADI();

void ACA();
void ACB();
void ACC();
void ACD();
void ACE();
void ACH();
void ACL();
void ACM();
void ACI();

void SUA();
void SUB();
void SUC();
void SUD();
void SUE();
void SUH();
void SUL();
void SUM();
void SUI();

void SBA();
void SBB();
void SBC();
void SBD();
void SBE();
void SBH();
void SBL();
void SBM();
void SBI();

void NDA();
void NDB();
void NDC();
void NDD();
void NDE();
void NDH();
void NDL();
void NDM();
void NDI();

void XRA();
void XRB();
void XRC();
void XRD();
void XRE();
void XRH();
void XRL();
void XRM();
void XRI();

void ORA();
void ORB();
void ORC();
void ORD();
void ORE();
void ORH();
void ORL();
void ORM();
void ORI();

void CPA();
void CPB();
void CPC();
void CPD();
void CPE();
void CPH();
void CPL();
void CPM();
void CPI();

void INB();
void INC();
void IND();
void INE();
void INH();
void INL();

void DCB();
void DCC();
void DCD();
void DCE();
void DCH();
void DCL();

void RLC();
void RAL();
void RRC();
void RAR();
