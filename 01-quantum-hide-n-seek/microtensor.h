/**
 * @file microtensor.h
 * @author Rongcui Dong (rongcuid@outlook.com)
 * @brief A super simple library for a small subset of single float tensor
 * operations.
 * @version 0.1
 * @date 2026-05-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <stddef.h>

#include <msc.h>

typedef struct {
  ptrdiff_t ndims;
  ptrdiff_t *items;
} mt_shape_t;

typedef struct {
  ptrdiff_t ndims;
  ptrdiff_t *items;
} mt_index_t;

typedef struct {
  ptrdiff_t size;
  float *data;
} mt_data_t;

typedef struct {
  mt_shape_t shape;
  mt_data_t data;
} mt_view_t;

/**
 * @brief Compute the data index.
 *
 * @param ndims
 * @param shape
 * @param indices
 * @return ptrdiff_t
 */
ptrdiff_t mt_index(int ndims, const ptrdiff_t shape[],
                   const ptrdiff_t indices[]);

/**
 * @brief Allocate data for a tensor.
 *
 * @return msc_err_t
 */
msc_err_t mt_alloc(mt_shape_t shape, mt_data_t *data,
                   const msc_allocator_t *alloc);

void mt_free(mt_data_t *data, const msc_allocator_t *alloc);