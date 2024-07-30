#ifndef __GX_COMP_H__
#define __GX_COMP_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define make_comp(x) sizeof(x)

#define gx_comp_storage_new(numb_component, ...)                               \
  __gx_comp_storage_new(numb_component, __VA_ARGS__);

typedef struct {
  uint8_t numb_comp;
  size_t *size_comp;
  size_t *size;
  size_t *cap;
  void **comp;
} GxCompStorage;

GxCompStorage __gx_comp_storage_new(int numb_component, ...);
void *gx_comp_storage_insert(GxCompStorage *cs, uint8_t type,
                             const void *const comp);
void gx_comp_storage_remove(GxCompStorage *cs, uint8_t type, size_t pos);

void gx_comp_destroy(GxCompStorage *cs);

#ifdef __cplusplus
}
#endif

#endif //__GX_COMP_H__
