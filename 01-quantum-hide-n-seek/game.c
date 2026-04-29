#include "game.h"

msc_err_t board_init(board_state_t *board, ptrdiff_t m, ptrdiff_t n,
                     const msc_allocator_t *alloc) {
  msc_err_t err = MSC_ERR;
  float *probabilities = nullptr;
  bool *is_exit = nullptr;
  if ((probabilities = msc_malloc(alloc, sizeof(float) * m * n,
                                  _Alignof(float))) == nullptr) {
    err = MSC_NOMEM;
    goto err;
  }
  if ((is_exit = msc_malloc(alloc, sizeof(bool) * m * n, _Alignof(bool))) ==
      nullptr) {
    err = MSC_NOMEM;
    goto err;
  }
  *board = (board_state_t){
      .alloc = alloc,
      .m = m,
      .n = n,
      .probabilities = probabilities,
      .is_exit = is_exit,
  };
  err = MSC_OK;
  goto end;
err:
  if (is_exit != nullptr) {
    msc_free(alloc, is_exit, sizeof(bool) * m * n, _Alignof(bool));
  }
  if (probabilities != nullptr) {
    msc_free(alloc, probabilities, sizeof(float) * m * n, _Alignof(bool));
  }
end:
  return err;
}

void board_deinit(board_state_t *board) {
  msc_free(board->alloc, board->probabilities,
           sizeof(float) * board->m * board->n, _Alignof(float));
  msc_free(board->alloc, board->is_exit, sizeof(bool) * board->m * board->n,
           _Alignof(bool));
  *board = (board_state_t){0};
}

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