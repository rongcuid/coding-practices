#ifndef MSCC_ARENA_H_
#define MSCC_ARENA_H_

#include <stddef.h>

typedef struct {
  char *begin;
  char *end;
} mscc_arena_t;

mscc_arena_t mscc_arena_from_buf(void *buf, ptrdiff_t capacity);
mscc_arena_t mscc_arena_suballoc(mscc_arena_t *a, ptrdiff_t capacity);

void *mscc_arena_malloc(mscc_arena_t *a, ptrdiff_t size, ptrdiff_t align);
void mscc_arena_free(mscc_arena_t *a, void *ptr, ptrdiff_t size,
                     ptrdiff_t align);
void *mscc_arena_realloc(mscc_arena_t *a, void *ptr, ptrdiff_t old_size,
                         ptrdiff_t new_size, ptrdiff_t align);

#endif