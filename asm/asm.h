#pragma once
#include <sys/types.h>

// sizeof(vec_entry) = size of opcode = size of macro
struct vec_entry
{                       // for opcodes          for macros
    u_int16_t a;        // opcode               value
    u_int16_t s;        // size                 for ALI,ORG: new pc
    char * b;           // arg                  name
};

struct vector
{
    struct vec_entry * data;
    size_t cap, size;
};

void attach(struct vector * v);
void detach(struct vector * v);
void push(struct vector * v, struct vec_entry e);
