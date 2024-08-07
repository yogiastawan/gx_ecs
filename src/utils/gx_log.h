#ifndef __GX_LOG_H__
#define __GX_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ANDROID__
// TODO! add log for android
#define log_i(...)
#define log_e(...)

#elif __linux__
#include <stdio.h>

#define log_i(...)                                                             \
  printf("GX_INFO:: ");                                                        \
  printf(__VA_ARGS__)
#define log_e(...)                                                             \
  fprintf(stderr, "GX_ERR:: ");                                                \
  fprintf(stderr, __VA_ARGS__)

#elif _WIN32
#include <stdio.h>

#define log_i(...)                                                             \
  printf("GX_INFO:: ");                                                        \
  printf(__VA_ARGS__)
#define log_e(...)                                                             \
  fprintf(stderr, "GX_ERR:: ");                                                \
  fprintf(stderr, __VA_ARGS__)

#else
#include <stdio.h>

#define log_i(...)                                                             \
  printf("GX_INFO:: ");                                                        \
  printf(__VA_ARGS__)
#define log_e(...)                                                             \
  fprintf(stderr, "GX_ERR:: ");                                                \
  fprintf(stderr, __VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#endif //__GX_LOG_H__
