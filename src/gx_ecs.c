#include "gx_ecs.h"
#include "gx_comp.h"
#include "gx_entity.h"
#include "utils/gx_dllist.h"

#include <stdarg.h>
#include <stdlib.h>

GxEcs *gx_ecs_new() {
  GxEcs *ecs = malloc(sizeof(GxEcs));
  ecs->entt = malloc(sizeof(GxEntityCore));
  ecs->store = malloc(sizeof(GxCompStorage));
  return ecs;
}
void gx_ecs_init(GxEcs *ecs, int numb_component, ...) {
  va_list args;
  va_start(args, numb_component);
  *ecs->entt = gx_entity_core_new(numb_component);
  *ecs->store = gx_comp_storage_new(numb_component, args);
  va_end(args);
}

void gx_ecs_destroy(GxEcs *ecs) {
  gx_entity_core_destroy(ecs->entt);
  gx_comp_storage_destroy(ecs->store);
  free(ecs->entt);
  ecs->entt = NULL;
  free(ecs->store);
  ecs->store = NULL;
}

GxEntity gx_ecs_create_entity(GxEcs *ecs, int numb_comp, ...) {
  va_list args;
  va_start(args, numb_comp);

  uint8_t comp_type = va_arg(args, int);
  void *comp = va_arg(args, void *);
  GxNode *node = gx_comp_storage_insert(ecs->store, comp_type, comp);
  GxEntity entt = gx_entity_core_insert(ecs->entt, comp_type, node);

  for (uint8_t i = 2; i < numb_comp * 2; i += 2) {
    uint8_t comp_type = va_arg(args, int);
    void *comp = va_arg(args, void *);
    GxNode *node = gx_comp_storage_insert(ecs->store, comp_type, comp);
    gx_entity_core_add(ecs->entt, entt, comp_type, node);
  }

  va_end(args);
  return entt;
}
void gx_ecs_delete_entity(GxEcs *ecs, GxEntity entt) {
  for (uint8_t i = 0; i < ecs->entt->numb_comp; i++) {
    GxNode *node = gx_entity_core_remove_comp(ecs->entt, entt, i);
    gx_comp_storage_remove(ecs->store, i, node);
  }
}

bool gx_ecs_insert_component(GxEcs *ecs, GxEntity entt, uint8_t comp_type,
                             const void *const comp) {
  GxNode *node = gx_comp_storage_insert(ecs->store, comp_type, comp);
  return gx_entity_core_add(ecs->entt, entt, comp_type, node);
}

void gx_ecs_delete_component(GxEcs *ecs, GxEntity entt, uint8_t comp_type) {
  GxNode *node = gx_entity_core_remove_comp(ecs->entt, entt, comp_type);
  gx_comp_storage_remove(ecs->store, comp_type, node);
}

GxQueryComp *gx_ecs_query(const GxEcs *const ecs, int n, ...) {
  va_list args;
  va_start(args, n);
  GxQueryComp *q = gx_query_comp_get(ecs->entt, n, args);
  va_end(args);
  return q;
}
GxQueryCompEntity *gx_ecs_query_entity(const GxEcs *const ecs, int n, ...) {
  va_list args;
  va_start(args, n);
  GxQueryCompEntity *q = gx_query_comp_entity_get(ecs->entt, n, args);
  va_end(args);
  return q;
}
