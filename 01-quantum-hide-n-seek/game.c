#include "game.h"

observation_result_t observe_hider(const board_state_t *board,
                                   const observation_t *observation,
                                   const msc_allocator_t *up,
                                   msc_arena_t scratch) {
  observation_result_t result = {0};
  // TODO
  return result;
}

void push_timeline(timeline_t *t, board_state_t *board,
                   observation_t *observation, msc_arena_t scratch) {}

void pop_timeline(timeline_t *t, board_state_t **board_state,
                  observation_t **observation) {}