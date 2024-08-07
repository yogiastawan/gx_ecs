#ifndef __GX_DEB_H__
#define __GX_DEB_H__

#include "utils/gx_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GX_DEBUG
#include <assert.h>

#define gx_assert(x, ...)                                                      \
  {                                                                            \
    if (x) {                                                                   \
      log_e(__VA_ARGS__);                                                      \
      assert(!(x));                                                            \
    }                                                                          \
  }
#else
#define gx_assert(x, msg)
#endif

#ifdef __cplusplus
}
#endif

#endif //__GX_CEB_H__
