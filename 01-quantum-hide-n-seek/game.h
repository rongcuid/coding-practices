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
#include <stdint.h>

#include <msc.h>

typedef struct {
  const msc_allocator_t *alloc;
  ptrdiff_t m;
  ptrdiff_t n;
  float *probabilities;
  bool *is_exit;
} board_state_t;

typedef struct {
  const msc_allocator_t *alloc;
  ptrdiff_t len;
  ptrdiff_t cap;
  ptrdiff_t *xs;
  ptrdiff_t *ys;
  int32_t *tags;
} observation_t;

typedef struct {
  board_state_t next_before_collapse;
  bool collapsed;
  bool exited;
  ptrdiff_t collapse_x;
  ptrdiff_t collapse_y;
} observation_result_t;

typedef struct {
  const msc_allocator_t *alloc;
  ptrdiff_t len;
  ptrdiff_t cap;
  board_state_t *board_states;
  observation_t *observations;
} timeline_t;

msc_err_t board_init(board_state_t *board, ptrdiff_t m, ptrdiff_t n,
                     const msc_allocator_t *alloc);
void board_deinit(board_state_t *board);

msc_err_t observation_init(observation_t *obs, ptrdiff_t initial_capacity,
                           const msc_allocator_t *alloc);
void observation_deinit(observation_t *obs);

msc_err_t timeline_init(timeline_t *tl, ptrdiff_t initial_capacity,
                        const msc_allocator_t *alloc);
void timeline_deinit(timeline_t *tl);

observation_result_t observe_hider(const board_state_t *board,
                                   const observation_t *observation,
                                   const msc_allocator_t *up,
                                   msc_arena_t scratch);

void push_timeline(timeline_t *t, board_state_t *board,
                   observation_t *observation, msc_arena_t scratch);

void pop_timeline(timeline_t *t, board_state_t **board_state,
                  observation_t **observation);