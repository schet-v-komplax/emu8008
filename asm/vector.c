#include "asm.h"
#include <stdlib.h>

void attach(struct vector * v)
{
    v->cap = 2;
    v->data = malloc(2 * sizeof(struct vec_entry));
}

void detach(struct vector * v)
{
    free(v->data);
}

void push(struct vector * v, struct vec_entry e)
{
    if(v->cap <= v->size) {
        v->cap += v->cap >> 1;
        v->data = realloc(v->data, v->cap * sizeof(struct vec_entry));
    }
    v->data[v->size++] = e;
}
