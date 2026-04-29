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

msc_err_t observation_init(observation_t *obs, ptrdiff_t initial_capacity,
                           const msc_allocator_t *alloc) {
  msc_err_t err = MSC_ERR;
  ptrdiff_t *xs = nullptr, *ys = nullptr;
  int32_t *tags = nullptr;
  if ((xs = msc_malloc(alloc, sizeof(ptrdiff_t) * initial_capacity,
                       _Alignof(ptrdiff_t)))) {
    err = MSC_NOMEM;
    goto err;
  }
  if ((ys = msc_malloc(alloc, sizeof(ptrdiff_t) * initial_capacity,
                       _Alignof(ptrdiff_t)))) {
    err = MSC_NOMEM;
    goto err;
  }
  if ((tags = msc_malloc(alloc, sizeof(int32_t) * initial_capacity,
                         _Alignof(int32_t)))) {
    err = MSC_NOMEM;
    goto err;
  }
  *obs = (observation_t){.alloc = alloc,
                         .len = 0,
                         .cap = initial_capacity,
                         .xs = xs,
                         .ys = ys,
                         .tags = tags};
  err = MSC_OK;
  goto end;
err:
  if (xs != nullptr) {
    msc_free(alloc, xs, sizeof(ptrdiff_t) * initial_capacity,
             _Alignof(ptrdiff_t));
  }
  if (ys != nullptr) {
    msc_free(alloc, ys, sizeof(ptrdiff_t) * initial_capacity,
             _Alignof(ptrdiff_t));
  }
  if (tags != nullptr) {
    msc_free(alloc, tags, sizeof(int32_t) * initial_capacity,
             _Alignof(int32_t));
  }
end:
  return err;
}

void observation_deinit(observation_t *obs) {
  msc_free(obs->alloc, obs->xs, sizeof(ptrdiff_t) * obs->cap,
           _Alignof(ptrdiff_t));
  msc_free(obs->alloc, obs->ys, sizeof(ptrdiff_t) * obs->cap,
           _Alignof(ptrdiff_t));
  msc_free(obs->alloc, obs->tags, sizeof(int32_t) * obs->cap,
           _Alignof(int32_t));
  *obs = (observation_t){0};
}

msc_err_t timeline_init(timeline_t *tl, ptrdiff_t initial_capacity,
                        const msc_allocator_t *alloc) {
  msc_err_t err = MSC_ERR;
  board_state_t *board_states = nullptr;
  observation_t *observations = nullptr;
  if ((board_states =
           msc_malloc(alloc, sizeof(board_state_t) * initial_capacity,
                      _Alignof(board_state_t))) == nullptr) {
    err = MSC_NOMEM;
    goto err;
  }
  if ((observations =
           msc_malloc(alloc, sizeof(observation_t) * initial_capacity,
                      _Alignof(observation_t))) == nullptr) {
    err = MSC_NOMEM;
    goto err;
  }
  *tl = (timeline_t){.alloc = alloc,
                     .len = 0,
                     .cap = initial_capacity,
                     .board_states = board_states,
                     .observations = observations};
  err = MSC_OK;
  goto end;
err:
  if (observations != nullptr) {
    msc_free(alloc, observations, sizeof(observation_t) * initial_capacity,
             _Alignof(observation_t));
  }
  if (board_states != nullptr) {
    msc_free(alloc, board_states, sizeof(board_state_t) * initial_capacity,
             _Alignof(board_state_t));
  }
end:
  return err;
}
void timeline_deinit(timeline_t *tl) {
  msc_free(tl->alloc, tl->observations, sizeof(observation_t) * tl->cap,
           _Alignof(observation_t));
  msc_free(tl->alloc, tl->board_states, sizeof(board_state_t) * tl->cap,
           _Alignof(board_state_t));
  *tl = (timeline_t){0};
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
