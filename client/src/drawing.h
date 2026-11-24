#ifndef DRAWING_H
#define DRAWING_H

typedef struct Drawing {
    int z;
    void (*draw)(void* self);
} Drawing;

#endif