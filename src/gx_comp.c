#include "gx_comp.h"
#include "utils/gx_dllist.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

GxCompStorage __gx_comp_storage_new(int numb_component, ...) {
  GxCompStorage cs = {.numb_comp = (uint8_t)numb_component,
                      .comp = malloc(sizeof(GxDLList) * numb_component)};
  va_list size;
  va_start(size, numb_component);
  for (uint8_t i = 0; i < numb_component; i++) {
    size_t s = va_arg(size, size_t);

    cs.comp[i] = gx_dllist_new(s);
  }
  va_end(size);

  return cs;
}
void *gx_comp_storage_insert(GxCompStorage *cs, uint8_t type,
                             const void *const comp) {
  if (type >= cs->numb_comp) {
    return NULL;
  }

  gx_dllist_append(&cs->comp[type], comp);
  return cs->comp[type].last_node;
}

bool gx_comp_storage_remove(GxCompStorage *cs, uint8_t type, size_t pos) {
  if (type >= cs->numb_comp) {
    // Component type is out of range
    return false;
  }

  // TODO!
  // Get address component using variable post
  void *addr = NULL;
  gx_dllist_delete(&cs->comp[type], addr);

  return true;
}

void gx_comp_destroy(GxCompStorage *cs) {

  for (uint8_t i = 0; i < cs->numb_comp; i++) {
    gx_dllist_destroy(&cs->comp[i]);
  }

  free(cs->comp);
  cs->comp = NULL;
}
