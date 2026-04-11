#include "alloc.h"

#include <assert.h>
#include <stdlib.h>

void *mscc_malloc(const mscc_allocator_t *alloc, ptrdiff_t size,
                  ptrdiff_t align) {
  assert(alloc != nullptr);
  assert(alloc->malloc != nullptr);
  assert(size >= 0);
  assert(align >= 0);
  return alloc->malloc(alloc->context, size, align);
}

void *mscc_realloc(const mscc_allocator_t *alloc, void *ptr, ptrdiff_t old_size,
                   ptrdiff_t old_align, ptrdiff_t new_size,
                   ptrdiff_t new_align) {
  assert(alloc != nullptr);
  assert(alloc->realloc != nullptr);
  assert(old_size >= 0);
  assert(old_align >= 0);
  assert(new_size >= old_size);
  assert(new_align >= 0);
  return alloc->realloc(alloc->context, ptr, old_size, old_align, new_size,
                        new_align);
}

void mscc_free(const mscc_allocator_t *alloc, void *ptr, ptrdiff_t size,
               ptrdiff_t align) {
  assert(alloc != nullptr);
  assert(alloc->free != nullptr);
  assert(size >= 0);
  assert(align >= 0);
  alloc->free(alloc->context, ptr, size, align);
}

void *c_malloc(void *context, ptrdiff_t size, ptrdiff_t align) {
  (void)context;
  (void)align;
  return malloc(size);
}

void *c_realloc(void *context, void *ptr, ptrdiff_t old_size,
                ptrdiff_t old_align, ptrdiff_t new_size, ptrdiff_t new_align) {
  (void)context;
  (void)old_size;
  (void)old_align;
  (void)new_align;
  return realloc(ptr, new_size);
}

void c_free(void *context, void *ptr, ptrdiff_t size, ptrdiff_t align) {
  (void)context;
  (void)size;
  (void)align;
  free(ptr);
}

const mscc_allocator_t mscc_c_allocator = {
    .malloc = &c_malloc,
    .realloc = &c_realloc,
    .free = &c_free,
};