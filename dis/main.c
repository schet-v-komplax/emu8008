#include "../etc/common.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

static char * opcode_map[] = {
    "HLT",  "HLT",  "RLC",  "RFC",  "ADI",  "RST0", "LAI",  "RET",  "INB",  "DCB",  "RRC",  "RFZ",  "ACI",  "RST1", "LBI",  "RET",
    "INC",  "DCC",  "RAL",  "RFS",  "SUI",  "RST2", "LCI",  "RET",  "IND",  "DCD",  "RAR",  "RFP",  "SBI",  "RST3", "LDI",  "RET",
    "INE",  "DCE",  0,      "RTC",  "NDI",  "RST4", "LEI",  "RET",  "INH",  "DCH",  0,      "RTZ",  "XRI",  "RST5", "LHI",  "RET",
    "INL",  "DCL",  0,      "RTS",  "ORI",  "RST6", "LLI",  "RET",  0,      0,      0,      "RTP",  "CPI",  "RST7", "LMI",  "RET",
    "JFC",  "INP0", "CFC",  "INP1", "JMP",  "INP2", "CAL",  "INP3", "JFZ",  "INP4", "CFZ",  "INP5", "JMP",  "INP6", "CAL",  "INP7",
    "JFS",  "OUT8", "CFS",  "OUT9", "JMP",  "OUT10","CAL",  "OUT11","JFP",  "OUT12","CFP",  "OUT13","JMP",  "OUT14","CAL",  "OUT15",
    "JTC",  "OUT16","CTC",  "OUT17","JMP",  "OUT18","CAL",  "OUT19","JTZ",  "OUT20","CTZ",  "OUT21","JMP",  "OUT21","CAL",  "OUT23",
    "JTS",  "OUT24","CTS",  "OUT25","JMP",  "OUT26","CAL",  "OUT27","JTP",  "OUT28","CTP",  "OUT29","JMP",  "OUT30","CAL",  "OUT31",
    "ADA",  "ADB",  "ADC",  "ADD",  "ADE",  "ADH",  "ADL",  "ADM",  "ACA",  "ACB",  "ACC",  "ACD",  "ACE",  "ACH",  "ACL",  "ACM",
    "SUA",  "SUB",  "SUC",  "SUD",  "SUE",  "SUH",  "SUL",  "SUM",  "SBA",  "SBB",  "SBC",  "SBD",  "SBE",  "SBH",  "SBL",  "SBM",
    "NDA",  "NDB",  "NDC",  "NDD",  "NDE",  "NDH",  "NDL",  "NDM",  "XRA",  "XRB",  "XRC",  "XRD",  "XRE",  "XRH",  "XRL",  "XRM",
    "ORA",  "ORB",  "ORC",  "ORD",  "ORE",  "ORH",  "ORL",  "ORM",  "CPA",  "CPB",  "CPC",  "CPD",  "CPE",  "CPH",  "CPL",  "CPM",
    "NOP",  "LAB",  "LAC",  "LAD",  "LAE",  "LAH",  "LAL",  "LAM",  "LBA",  "LBB",  "LBC",  "LBD",  "LBE",  "LBH",  "LBL",  "LBM",
    "LCA",  "LCB",  "LCC",  "LCD",  "LCE",  "LCH",  "LCL",  "LCM",  "LDA",  "LDB",  "LDC",  "LDD",  "LDE",  "LDH",  "LDL",  "LDM",
    "LEA",  "LEB",  "LEC",  "LED",  "LEE",  "LEH",  "LEL",  "LEM",  "LHA",  "LHB",  "LHC",  "LHD",  "LHE",  "LHH",  "LHL",  "LHM",
    "LLA",  "LLB",  "LLC",  "LLD",  "LLE",  "LLH",  "LLL",  "LLM",  "LMA",  "LMB",  "LMC",  "LMD",  "LME",  "LMH",  "LML",  "HLT"
};

static u_int8_t opsize_map[] = {
    1,      1,      1,      1,      2,      1,      2,      1,      1,      1,      1,      1,      2,      1,      2,      1,
    1,      1,      1,      1,      2,      1,      2,      1,      1,      1,      1,      1,      2,      1,      2,      1,
    1,      1,      1,      1,      2,      1,      2,      1,      1,      1,      1,      1,      2,      1,      2,      1,
    1,      1,      1,      1,      2,      1,      2,      1,      1,      1,      1,      1,      2,      1,      2,      1,
    3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,
    3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,
    3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,
    3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,      3,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
    1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1
};

void usage()
{
    die("usage: %s <file>", getprogname());
}

int main(int argc, char * argv[])
{
    u_int8_t buf[16 * 1024];
    char * opn;
    size_t s;
    int fd;
    u_int8_t skip;
    u_int8_t op;
    size_t i = 0;
    u_int8_t ops = 0;
    bool preveq = false;

    setprogname(argv[0]);
    if(argc != 2) usage();
    
    fd = open(argv[1], O_RDONLY);
    if(fd < 0) usage();

    s = read(fd, buf, 0x4000);
    close(fd);

    while(i < s) {
        if(ops && !memcmp(buf + i - ops, buf + i, ops)) {
            if(!preveq) printf("*\n");
            preveq = true;
            i += ops;
            continue;
        }
        preveq = false;
        op = buf[i];
        ops = opsize_map[op];
        opn = opcode_map[op];
        printf("%04lx\t", i);
        
        skip = ops;
        while(skip--) printf("%02x ", buf[i++]);
        skip = 4 - ops;
        while(skip--) printf("   ");

        if(!opn) printf("\t<unknown>");
        else {
            printf("\t%.3s", opn); 
            if(strlen(opn) > 3) printf("\t%s", opn + 3);
        }
        if(ops == 2) printf("\t%#x", buf[i - 1]);
        else if(ops == 3) printf("\t%#x", *(u_int16_t *)(buf + i - 2));
        printf("\n");
    }
    return 0;
}
