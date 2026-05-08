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

msc_err_t mt_alloc(mt_shape_t shape, mt_data_t *data,
                   const msc_allocator_t *alloc) {
  msc_err_t err = MSC_ERR;
  float *tensor_data = nullptr;
  if (shape.ndims < 0) {
    err = MSC_BADARGS;
    goto err;
  }
  if (shape.ndims == 0) {
    *data = (mt_data_t){.len = 0, .items = nullptr};
    goto ok_zero;
  }
  ptrdiff_t size = 1;
  for (ptrdiff_t i = 0; i < shape.ndims; ++i) {
    size *= shape.items[i];
  }
  if (size < 0) {
    err = MSC_BADARGS;
    goto err;
  }
  if (size == 0) {
    goto ok_zero;
  }
  tensor_data = msc_malloc(alloc, size * sizeof(float), _Alignof(float));
  if (tensor_data == nullptr) {
    err = MSC_NOMEM;
    goto err;
  }
  *data = (mt_data_t){.len = size, .items = tensor_data};
  goto ok;
err:
  if (tensor_data != nullptr) {
    msc_free(alloc, tensor_data, size * sizeof(float), _Alignof(float));
  }
  goto end;
ok_zero:
  *data = (mt_data_t){.len = 0, .items = nullptr};
ok:
  err = MSC_OK;
end:
  return err;
}

void mt_free(mt_data_t *data, const msc_allocator_t *alloc) {
  msc_free(alloc, data->items, data->len * sizeof(float), _Alignof(float));
  data->items = nullptr;
  data->len = 0;
}

void mt_fill(mt_view_t tensor, float value) {
  for (ptrdiff_t i = 0; i < tensor.data.len; ++i) {
    tensor.data.items[i] = value;
  }
}