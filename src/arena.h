#ifndef ARENA_H
#define ARENA_H

#include "allat.h"
#include "string.h"

typedef enum flags
{
    NOZERO = 0,
    MEMZERO = 1
} Flags;

typedef struct arena // 24 byte wide
{
    U64 size;
    U64 offset;
    C8 *buf;
} Arena;

/* == Create / Destory == */

Arena *arena_alloc(U64 size);
U0 arena_release(Arena *arena);

/* == END Create / Destroy == */


/* == Allocate / Deallocate == */

U0 *arena_push(Arena *arena, U64 alignment, U64 size, Flags flags);
U0 *arena_pop(Arena* arena, U64 size, Flags flags);

/* == END Allocate / Deallocate */

#define new(a, t, n, f) (t*)arena_push(a, alignof(t), (n) * sizeof(t), (f))

#endif
