#include "gx_comp.h"
#include "utils/gx_deb.h"
#include "utils/gx_dllist.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

GxCompStorage gx_comp_storage_new(int numb_component, va_list comps) {
  GxCompStorage cs = {.numb_comp = (uint8_t)numb_component,
                      .comp = malloc(sizeof(GxDLList) * numb_component)};

  for (uint8_t i = 0; i < numb_component; i++) {
    size_t s = va_arg(comps, size_t);

    cs.comp[i] = gx_dllist_new(s);
  }

  return cs;
}
GxNode *gx_comp_storage_insert(GxCompStorage *cs, uint8_t type,
                               const void *const comp) {
  if (type >= cs->numb_comp) {
    return NULL;
  }

  gx_dllist_append(&cs->comp[type], comp);
  return cs->comp[type].last_node;
}

void gx_comp_storage_remove(GxCompStorage *cs, uint8_t type,
                            GxNode *node_comp) {

  gx_assert(type >= cs->numb_comp,
            "Accessing type component %u when max number components is %u",
            type, cs->numb_comp);

  gx_dllist_delete(&cs->comp[type], node_comp);
}

void gx_comp_storage_destroy(GxCompStorage *cs) {

  for (uint8_t i = 0; i < cs->numb_comp; i++) {
    gx_dllist_destroy(&cs->comp[i]);
  }

  free(cs->comp);
  cs->comp = NULL;
}
