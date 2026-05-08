#include "game.h"

#include "microtensor.h"

msc_err_t board_init(board_state_t *board, ptrdiff_t m, ptrdiff_t n,
                     const msc_allocator_t *alloc) {
  msc_err_t err = MSC_ERR;
  float *scores = nullptr;
  bool *is_exit = nullptr;
  if ((scores = msc_malloc(alloc, sizeof(float) * m * n, _Alignof(float))) ==
      nullptr) {
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
      .scores = scores,
      .is_exit = is_exit,
  };
  err = MSC_OK;
  goto end;
err:
  if (is_exit != nullptr) {
    msc_free(alloc, is_exit, sizeof(bool) * m * n, _Alignof(bool));
  }
  if (scores != nullptr) {
    msc_free(alloc, scores, sizeof(float) * m * n, _Alignof(bool));
  }
end:
  return err;
}

void board_deinit(board_state_t *board) {
  msc_free(board->alloc, board->scores, sizeof(float) * board->m * board->n,
           _Alignof(float));
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

/**
 * @brief Calculate the update gradient of the board given board state and
 * observations.
 *
 * @param board
 * @param observation
 * @param dsdu
 * @param dsdv
 */
static void board_gradient(const board_state_t *board,
                           const observation_t *observation, float *dsdu,
                           float *dsdv) {
  const ptrdiff_t board_shape[] = {board->m, board->n};
  const ptrdiff_t grad_shape[] = {board->m - 1, board->n - 1};
  for (ptrdiff_t i = 1; i < board_shape[0] - 1; ++i) {
    for (ptrdiff_t j = 1; j < board_shape[1] - 1; ++j) {
      // Useful indices.
      ptrdiff_t ij = mt_index(2, board_shape, (ptrdiff_t[]){i, j});
      // Exits never diffuse score.
      if (board->is_exit[ij]) {
        continue;
      }
      ptrdiff_t ij_i_lo = mt_index(2, board_shape, (ptrdiff_t[]){i - 1, j});
      ptrdiff_t ij_i_hi = mt_index(2, board_shape, (ptrdiff_t[]){i + 1, j});
      ptrdiff_t ij_j_lo = mt_index(2, board_shape, (ptrdiff_t[]){i, j - 1});
      ptrdiff_t ij_j_hi = mt_index(2, board_shape, (ptrdiff_t[]){i, j + 1});
      // If cell is adjacent to one or more exits, score is always diffused into
      // exits.
      // If cell is observed, score is cleared and redistributed evenly.
    }
  }
}

// TODO: currently doesn't handle exits.
msc_err_t observe_hider(const board_state_t *board,
                        const observation_t *observation,
                        observation_result_t *result, const msc_allocator_t *up,
                        msc_arena_t scratch) {
  msc_err_t err = MSC_ERR;
  // We calculate gradient on the internal grid only, so it's a (M-1) x (N-1)
  // matrix. Gradient is computed on a staggered grid.
  // U-component corresponds to the i-component.
  float *dsdu = msc_arena_malloc(
      &scratch, sizeof(float) * (board->m - 1) * (board->n - 1),
      _Alignof(float));
  if (dsdu == nullptr) {
    err = MSC_NOMEM;
    goto err;
  }
  // V-component corresponds to the j-component.
  float *dsdv = msc_arena_malloc(
      &scratch, sizeof(float) * (board->m - 1) * (board->n - 1),
      _Alignof(float));
  if (dsdv == nullptr) {
    err = MSC_NOMEM;
    goto err;
  }
  // Compute the gradient.
  const ptrdiff_t board_shape[] = {board->m, board->n};
  const ptrdiff_t grad_shape[] = {board->m - 1, board->n - 1};
  board_gradient(board, observation, dsdu, dsdv);
  // Update score.
  for (ptrdiff_t i = 0; i < board_shape[0]; ++i) {
    for (ptrdiff_t j = 0; j < board_shape[1]; ++j) {
      float grad = 0;
      ptrdiff_t ij = mt_index(2, board_shape, (ptrdiff_t[]){i, j});
      ptrdiff_t u_0 = mt_index(2, grad_shape, (ptrdiff_t[]){i - 1, j});
      ptrdiff_t u_1 = mt_index(2, grad_shape, (ptrdiff_t[]){i, j});
      ptrdiff_t v_0 = mt_index(2, grad_shape, (ptrdiff_t[]){i, j - 1});
      ptrdiff_t v_1 = mt_index(2, grad_shape, (ptrdiff_t[]){i, j});
      grad += u_0 < 0 ? 0 : -dsdu[u_0];
      grad += u_1 < 0 ? 0 : dsdu[u_1];
      grad += v_0 < 0 ? 0 : -dsdv[v_0];
      grad += v_1 < 0 ? 0 : dsdv[v_1];
    }
  }
  // Done
  err = MSC_OK;
  goto end;
err:
end:
  return err;
}

void push_timeline(timeline_t *t, board_state_t *board,
                   observation_t *observation, msc_arena_t scratch) {}

void pop_timeline(timeline_t *t, board_state_t **board_state,
                  observation_t **observation) {}
