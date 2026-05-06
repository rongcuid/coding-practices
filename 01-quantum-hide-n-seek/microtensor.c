#include "microtensor.h"

ptrdiff_t mt_index(int ndims, const ptrdiff_t shape[],
                   const ptrdiff_t indices[]) {
  if (ndims < 0) {
    return -1;
  }
  ptrdiff_t index = 0;
  ptrdiff_t mult = 1;
  // Tensors are stored in row-major order, i.e. lowest index is contiguous.
  for (int d = ndims - 1; d >= 0; --d) {
    ptrdiff_t i = indices[d];
    ptrdiff_t s = shape[d];
    if (i < 0 || i >= s || s < 0) {
      return -1;
    }
    index += mult * i;
    mult *= s;
  }
  return index;
}