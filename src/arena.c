#include "arena.h"

Arena *arena_alloc(U64 size)
{
    Arena *arena;

    arena = (Arena*)malloc(sizeof(Arena));
    if (!arena)
    {
        fprintf(stderr, "Failed to allocate arena.\n");
        return NULL;
    }

    arena->buf = malloc(size);
    if (!arena->buf)
    {
        fprintf(stderr, "Failed to allocate arena buffer.\n");
        return NULL;
    }

    arena->size = size;
    arena->offset = 0;

    return arena;
}

U0 arena_release(Arena *arena)
{
    if (arena)
    {
        free(arena->buf);
        free(arena);
    }
}

U0 *arena_push(Arena *arena, U64 alignment, U64 size, Flags flags)
{
    U64 aligned_offset;

    aligned_offset = (arena->offset + alignment - 1) & ~(alignment - 1);

    if (aligned_offset > arena->size)
    {
        fprintf(stderr, "Arena out of memory.\n");
        return NULL;
    }

    arena->offset = aligned_offset + size;
    return flags & MEMZERO
        ? memset(&aligned_offset, 0, size)
        : arena->buf + aligned_offset;
}

U0 *arena_pop(Arena *arena, U64 size, Flags flags)
{
    if (!arena)
        return NULL;

    if (size > arena->size) size = arena->size;
    arena->offset -= size >= arena->offset ? arena->offset : size;

    return flags & NOZERO
        ? arena->buf + arena->offset
        : memset(arena->buf + arena->offset, 0, size);
}
