#include "emu8008.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

u_int8_t code[16 * 1024];
u_int16_t stack[8];

u_int16_t pc = 0;
u_int16_t sp = 8;

u_int8_t flags[4];
u_int16_t regs[7];

enum state state = RUN;

static int speed = 50;              // cycles per milisecond
static struct timeval time;

void cycles(int i)
{
    struct timeval new;
    int diff = speed * i * 1000;
    //fprintf(stderr, "cycles %d\n", i);
    // do {
    //     gettimeofday(&new, NULL);
    // } while((unsigned)(new.tv_usec - time.tv_usec) < (unsigned)diff);
    // time = new;
}

u_int8_t * m_reg()
{
    return (u_int8_t *)(code + (regs[5] << 8) + regs[6]);
}

typedef void (* instruction)();

static instruction instruction_map[] = {
    HLT,    HLT,    RLC,    RFC,    ADI,    RST0,   LAI,    RET,    INB,    DCB,    RRC,    RFZ,    ACI,    RST1,   LBI,    RET,
    INC,    DCC,    RAL,    RFS,    SUI,    RST2,   LCI,    RET,    IND,    DCD,    RAR,    RFP,    SBI,    RST3,   LDI,    RET,
    INE,    DCE,    0,      RTC,    NDI,    RST4,   LEI,    RET,    INH,    DCH,    0,      RTZ,    XRI,    RST5,   LHI,    RET,
    INL,    DCL,    0,      RTS,    ORI,    RST6,   LLI,    RET,    0,      0,      0,      RTP,    CPI,    RST7,   LMI,    RET,
    JFC,    INP0,   CFC,    INP1,   JMP,    INP2,   CAL,    INP3,   JFZ,    INP4,   CFZ,    INP5,   JMP,    INP6,   CAL,    INP7,
    JFS,    OUT8,   CFS,    OUT9,   JMP,    OUT10,  CAL,    OUT11,  JFP,    OUT12,  CFP,    OUT13,  JMP,    OUT14,  CAL,    OUT15,
    JTC,    OUT16,  CTC,    OUT17,  JMP,    OUT18,  CAL,    OUT19,  JTZ,    OUT20,  CTZ,    OUT21,  JMP,    OUT21,  CAL,    OUT23,
    JTS,    OUT24,  CTS,    OUT25,  JMP,    OUT26,  CAL,    OUT27,  JTP,    OUT28,  CTP,    OUT29,  JMP,    OUT30,  CAL,    OUT31,
    ADA,    ADB,    ADC,    ADD,    ADE,    ADH,    ADL,    ADM,    ACA,    ACB,    ACC,    ACD,    ACE,    ACH,    ACL,    ACM,
    SUA,    SUB,    SUC,    SUD,    SUE,    SUH,    SUL,    SUM,    SBA,    SBB,    SBC,    SBD,    SBE,    SBH,    SBL,    SBM,
    NDA,    NDB,    NDC,    NDD,    NDE,    NDH,    NDL,    NDM,    XRA,    XRB,    XRC,    XRD,    XRE,    XRH,    XRL,    XRM,
    ORA,    ORB,    ORC,    ORD,    ORE,    ORH,    ORL,    ORM,    CPA,    CPB,    CPC,    CPD,    CPE,    CPH,    CPL,    CPM,
    NOP,    LAB,    LAC,    LAD,    LAE,    LAH,    LAL,    LAM,    LBA,    LBB,    LBC,    LBD,    LBE,    LBH,    LBL,    LBM,
    LCA,    LCB,    LCC,    LCD,    LCE,    LCH,    LCL,    LCM,    LDA,    LDB,    LDC,    LDD,    LDE,    LDH,    LDL,    LDM,
    LEA,    LEB,    LEC,    LED,    LEE,    LEH,    LEL,    LEM,    LHA,    LHB,    LHC,    LHD,    LHE,    LHH,    LHL,    LHM,
    LLA,    LLB,    LLC,    LLD,    LLE,    LLH,    LLL,    LLM,    LMA,    LMB,    LMC,    LMD,    LME,    LMH,    LML,    HLT
};

int main(int argc, char * argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if(fd < 0) return 1;
    read(fd, code, 16 * 1024);
    close(fd);
    
    gettimeofday(&time, NULL);
    for(;;) {
        //fprintf(stderr, "opcode=%x\n", code[pc]);
        instruction_map[code[pc]]();
    }

    return 0;
}
