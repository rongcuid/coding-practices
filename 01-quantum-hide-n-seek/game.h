/**
 * @file game.h
 * @author Rongcui Dong (rongcuid@outllook.com)
 * @brief Core game rule of Quantum Hide And Seek.
 * @version 0.1
 * @date 2026-04-28
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <stddef.h>

#include <msc.h>

typedef struct {
  ptrdiff_t m;
  ptrdiff_t n;
  float *probabilities;
} hider_state_t;

typedef struct {
  ptrdiff_t len;
  ptrdiff_t *xs;
  ptrdiff_t *ys;
} observation_t;

typedef struct {
  hider_state_t next_before_collapse;
  bool collapsed;
  ptrdiff_t collapse_x;
  ptrdiff_t collapse_y;
} observation_result_t;

observation_result_t observe_hider(const hider_state_t *hider,
                                   const observation_t *observation_t,
                                   const mscc_allocator_t *up,
                                   mscc_arena_t scratch);