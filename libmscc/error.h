#ifndef MSCC_ERROR_H_
#define MSCC_ERROR_H_

typedef enum {
  MSCC_OK = 0,
  MSCC_ERR = 1,
  MSCC_NOMEM,
  MSCC_BADARGS,
} mscc_err_t;

#endif