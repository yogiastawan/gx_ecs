#ifndef __GX_COMP_H__
#define __GX_COMP_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "utils/gx_dllist.h"

#ifdef __cplusplus
extern "C" {
#endif

#define make_comp(x) sizeof(x)

typedef struct {
  uint8_t numb_comp;
  GxDLList *comp;
} GxCompStorage;

GxCompStorage gx_comp_storage_new(int numb_component, va_list comps);
GxNode *gx_comp_storage_insert(GxCompStorage *cs, uint8_t type,
                               const void *const comp);
bool gx_comp_storage_remove(GxCompStorage *cs, uint8_t type, GxNode *node_comp);

void gx_comp_storage_destroy(GxCompStorage *cs);

#ifdef __cplusplus
}
#endif

#endif //__GX_COMP_H__
