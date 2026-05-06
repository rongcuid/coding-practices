/**
 * @file microtensor.h
 * @author Rongcui Dong (rongcuid@outlook.com)
 * @brief A super simple library for a small subset of floating point tensor
 * operations.
 * @version 0.1
 * @date 2026-05-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <stddef.h>

ptrdiff_t mt_index(int ndims, const ptrdiff_t shape[],
                   const ptrdiff_t indices[]);