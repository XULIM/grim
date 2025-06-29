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

/* Creates an arena of the specified size.
 * Returns the arena allocated on success, NULL otherwise. */
Arena *arena_alloc(U64 size);


/* Releases the arena allocated. */
U0 arena_release(Arena *arena);

/* == END Create / Destroy == */


/* == Allocate / Deallocate == */

/* Allocates a memory in arena given the alignment and size.
 * The memory is allocated from the closest offset given the alignment.
 * Flags:
 * - NOZERO: does not zero the allocated memory.
 * - MEMZERO: zeroes the allocated memory.
 * Returns the address of the allocated memory on success, NULL otherwise.
 * */
U0 *arena_push(Arena *arena, U64 alignment, U64 size, Flags flags);


/* Pops a certain size out of arena.
 *
 * Flags:
 * - NOZERO: does not zero the memory freed.
 * - MEMZERO: zeroes the memory freed.
 * Returns a pointer to the newly freed buffer.
 */
U0 *arena_pop(Arena* arena, U64 size, Flags flags);

/* == END Allocate / Deallocate */


/* Creates a pointer to type t of size n in arena a. The parameter f denotes the
 * flag (MEMZERO to zero the memory allocated, use NOZERO otherwise). */
#define new(a, t, n, f) (t*)arena_push(a, alignof(t), (n) * sizeof(t), (f))

#endif
