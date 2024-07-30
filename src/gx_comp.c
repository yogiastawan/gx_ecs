#include "gx_comp.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

GxCompStorage __gx_comp_storage_new(int numb_component, ...) {
  GxCompStorage cs = {.numb_comp = (uint8_t)numb_component,
                      .size_comp = malloc(sizeof(size_t) * numb_component),
                      .size = malloc(sizeof(size_t) * numb_component),
                      .cap = malloc(sizeof(size_t) * numb_component),
                      .comp = malloc(sizeof(void *) * numb_component)};
  va_list size;
  va_start(size, numb_component);
  for (uint8_t i = 0; i < numb_component; i++) {
    size_t s = va_arg(size, size_t);
    cs.size_comp[i] = s;
    cs.size[i] = 0;
    cs.cap[i] = 8;
    cs.comp[i] = malloc(s * cs.cap[i]);
  }
  va_end(size);

  return cs;
}
void *gx_comp_storage_insert(GxCompStorage *cs, uint8_t type,
                             const void *const comp) {
  if (type >= cs->numb_comp) {
    return NULL;
  }

  size_t s = cs->size[type];
  size_t s_comp = cs->size_comp[type];
  if (s > cs->cap[type]) {
    cs->cap[type] *= 2;
    cs->comp[type] = realloc(cs->comp[type], s_comp * cs->cap[type]);
  }

  memcpy(cs->comp[type] + (s * s_comp), comp, s_comp);
  cs->size[type]++;
  return cs->comp[type] + (s * s_comp);
}

void gx_comp_storage_remove(GxCompStorage *cs, uint8_t type, size_t pos) {}

void gx_comp_destroy(GxCompStorage *cs) {
  free(cs->size_comp);
  cs->size_comp = NULL;
  free(cs->size);
  cs->size = NULL;
  free(cs->cap);
  cs->cap = NULL;
  for (uint8_t i = 0; i < cs->numb_comp; i++) {
    free(cs->comp[i]);
    cs->comp[i] = NULL;
  }

  free(cs->comp);
  cs->comp = NULL;
}
