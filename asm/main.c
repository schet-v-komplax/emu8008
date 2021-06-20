/*
 * preprocessor commands:
 * 
 * EQU v        set value of last macro to v
 * 
 * compiler commands:
 * opcode   name    usage
 * 0x100    DFB v   define byte
 * 0x101    DFW v   define word
 * 0x102    ORG v   pc=v
 * 0x103    ALI v   pc=(pc+v-1)/v*v
 * 
 * NOTE: macro name lenght must not be equal to 3
 */

#define DFB     0x100
#define DFW     0x101
#define ORG     0x102
#define ALI     0x103

#include "../etc/common.h"
#include "asm.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

struct vector macros;
struct vector opcodes;

static char * buf;
static char * tok;
static char * last_token;

static u_int16_t pc = 0;

char * next_token()
{
    char * c, * ret;
    if(!last_token) die("end of file not expected");
    do {
        while(isspace(*tok)) tok++;
        if(*tok == ';') while(*tok != '\n' && *tok != EOF) tok++;
    } while(isspace(*tok));

    if(*tok == EOF) return NULL;
    c = tok;
    while(!isspace(*c)) c++;
    *c = '\0';
    ret = tok;
    tok = ++c;
    last_token = ret;
    return ret;
}

u_int16_t value(char * s)
{
    size_t len;
    u_int16_t mul;
    u_int16_t res = 0;
    bool off = false;
    
    if(s[0] == '.') {
        s++;
        if(!*s) return pc;
        off = true;
    }
    if(isdigit(s[0])) {
        len = strlen(s);
        if(s[len - 1] == 'Q') {
            len--;
            mul = 8;
        }
        else mul = 10;

        while(len--) {
            if(!isdigit(*s)) die("Bad value: '%s'", s);
            res = res * mul - '0' + *s++;
        }
        if(off) res += pc;
        return res;
    }
    for(len = 0; len < macros.size; len++) {
        if(!strcmp(s, macros.data[len].b)) return macros.data[len].a;
    }
    die("Bad value: '%s'", s);
}

u_int8_t reg(u_int8_t c)
{
    if(c == 'H') return 5;
    if(c == 'L') return 6;
    if(c == 'M') return 7;
    c -= 'A';
    if(c > 4) return 8;
    return c;
}

void bad_command()
{
    die("Bad command: '%s'", last_token);
}

void cond(u_int16_t first, char state, char flag, char * arg)
{
    if(state == 'T') first |= 0040;
    else if(state != 'F') bad_command();
    switch(flag) {
        case 'C': break;
        case 'Z': first += 8;  break;
        case 'S': first += 16; break;
        case 'P': first += 24; break;
        default: bad_command();
    }
    push(&opcodes, (struct vec_entry) { first, arg ? 3 : 1, arg });
}

void math(u_int16_t first, char op)
{
    u_int16_t x;
    if(op == 'I') {
        push(&opcodes, (struct vec_entry) { first | 0004, 2, next_token() });
        return;
    }
    x = reg(op);
    if(x > 7) bad_command();
    push(&opcodes, (struct vec_entry) { first | x | 0200, 1, NULL });
}

bool make_mnemonic()
{
    u_int16_t next, tmp;
    size_t len;
    char * c = next_token();
    if(!c) return false;
    len = strlen(c);
    if(len != 3) {
        push(&macros, (struct vec_entry) { pc, 0, c });
        return true;
    }

    // preprocessor command
    if(!strcmp(c, "EQU"))
        macros.data[macros.size - 1].a = value(next_token());

    // compiler command
    else if(!strncmp(c, "DF", 2)) {
        push(&opcodes, (struct vec_entry) { DFB, 0, next_token() });
        if(c[2] == 'W') opcodes.data[opcodes.size - 1].a = DFW;
        if(c[2] != 'B') bad_command();
    }
    else if(!strcmp(c, "ORG")) {
        tmp = value(next_token());
        if(tmp < pc) die("Bad 'ORG %s'", last_token);
        pc = tmp;
        push(&opcodes, (struct vec_entry) { ORG, pc, NULL });
    }
    else if(!strcmp(c, "ALI"))  {
        tmp = value(next_token());
        pc = ((pc + tmp - 1) / tmp) * tmp;
        push(&opcodes, (struct vec_entry) { ALI, pc, NULL });
    }

    // assembler opcode
    else {
        c++;
        switch(c[-1]) {
            case 'A':               // ACx, ADx
                if(c[0] == 'C') math(0010, c[1]);
                else if(c[0] == 'D') math(0000, c[1]);
                else bad_command();
                break;
            case 'C':               // CAL, CPx, Cxx
                if(!strcmp(c, "AL")) push(&opcodes, (struct vec_entry) { 0106, 3, next_token() });
                else if(c[0] == 'P') math(0070, c[1]);
                else cond(0102, c[0], c[1], next_token());
                break;
            case 'D':               // DCx
                if(c[0] != 'C') bad_command();
                next = reg(c[1]);
                if(!next || next > 7) bad_command();
                push(&opcodes, (struct vec_entry) { 0001 | (next << 3), 1, NULL });
                break;
            case 'H':               // HLT
                if(strcmp(c, "LT")) bad_command();
                push(&opcodes, (struct vec_entry) { 0377, 1, NULL });
                break;
            case 'I':               // INP, INx
                if(c[0] != 'N') bad_command();
                if(c[1] == 'P') {
                    next = value(next_token());
                    if(next > 7) bad_command();
                    push(&opcodes, (struct vec_entry) { 0101 | (next << 1), 1, NULL });
                }
                else {
                    next = reg(c[1]);
                    if(next > 7 || !next) bad_command();
                    push(&opcodes, (struct vec_entry) { 0000 | (next << 3), 1, NULL });
                }
                break;
            case 'J':               // JMP, Jxx
                if(!strcmp(c, "MP")) push(&opcodes, (struct vec_entry) { 0104, 3, next_token() });
                else cond(0100, c[0], c[1], next_token());
                break;
            case 'L':               // LxI, Lxx
                next = reg(c[0]);
                if(next > 7) bad_command();
                next <<= 3;
                if(c[1] == 'I')
                    push(&opcodes, (struct vec_entry) { next | 0006, 2, next_token() });
                else {
                    tmp = reg(c[1]);
                    if(tmp > 7) bad_command();
                    tmp |= 0300;
                    push(&opcodes, (struct vec_entry) { tmp | next, 1, NULL });
                }
                break;
            case 'N':               // NDx
                if(c[0] != 'D') bad_command();
                math(0040, c[1]);
                break;
            case 'O':               // OUT, ORx
                if(c[0] == 'R') math(0060, c[1]);
                else if(!strcmp(c, "UT")) {
                    next = value(next_token());
                    if(next < 8 || next > 31) bad_command();
                    push(&opcodes, (struct vec_entry) { 0101 | (next << 1), 1, NULL });
                }
                else bad_command();
                break;
            case 'R':               // RET, RST, Rxx, RLC, RRC, RAL, RAR
                if(c[0] == 'A') {
                    if(c[1] == 'L') push(&opcodes, (struct vec_entry) { 0022, 1, NULL });
                    else if(c[1] == 'R') push(&opcodes, (struct vec_entry) { 0032, 1, NULL });
                    else bad_command();
                }
                else if(c[0] == 'L') {
                    if(c[1] != 'C') bad_command();
                    push(&opcodes, (struct vec_entry) { 0002, 1, NULL });
                }
                else if(c[0] == 'R') {
                    if(c[1] != 'C') bad_command();
                    push(&opcodes, (struct vec_entry) { 0012, 1, NULL });
                }
                else if(!strcmp(c, "ET")) push(&opcodes, (struct vec_entry) { 0007, 1, NULL });
                else if(!strcmp(c, "ST")) {
                    next = value(next_token());
                    if(next & 07770) bad_command();
                    push(&opcodes, (struct vec_entry) { 0005 | (next << 3), 1, NULL });
                }
                else cond(0003, c[0], c[1], NULL);
                break;              
            case 'S':               // SUx, SBx
                if(c[0] == 'U') math(0020, c[1]);
                else if(c[0] == 'B') math(0030, c[1]);
                else bad_command();
                break;
            case 'X':               // XRx
                if(c[0] != 'R') bad_command();
                math(0050, c[1]);
                break;
            default: bad_command();
        }
        pc += opcodes.data[opcodes.size - 1].s;
    }
    return true;
}

int main(int argc, char * argv[])
{
    int fd;
    struct stat s;
    struct vec_entry * e;
    u_int16_t buf2;
    setprogname(argv[0]);
    
    fd = open(argv[1], O_RDONLY);
    fstat(fd, &s);
    buf = malloc(s.st_size + 1);
    buf[s.st_size] = EOF;
    tok = buf;
    last_token = buf;
    read(fd, buf, s.st_size);
    close(fd);

    attach(&macros);
    attach(&opcodes);

    while(make_mnemonic());

    pc = 0;
    for(size_t i = 0; i < opcodes.size; i++) {
        e = &opcodes.data[i];
        if(e->a & 0xff00) {
            switch(e->a) {
                case DFB:
                    putc(value(e->b), stdout);
                    pc++;
                    break;
                case DFW:
                    buf2 = value(e->b);
                    fwrite(&buf2, 2, 1, stdout);
                    pc += 2;
                    break;
                case ORG:
                case ALI:
                    fseek(stdout, e->s, SEEK_SET);
                    pc = e->s;
                    break;
            }
        }
        else {
            putc(e->a, stdout);
            if(e->s > 1) {
                buf2 = value(e->b);
                fwrite(&buf2, e->s - 1, 1, stdout);
            }
            pc += e->s;
        }
    }
    
    detach(&macros);
    detach(&opcodes);
    return 0;
}
