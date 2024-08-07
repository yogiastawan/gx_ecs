#ifndef __GX_ECS_H__
#define __GX_ECS_H__

#include <stdint.h>
#include <sys/types.h>

#include "gx_comp.h"
#include "gx_entity.h"

#ifdef __cplusplus
extern "C" {
#endif

// typedef void (*VoidSystem)(void);

typedef struct {
  GxEntityCore *entt;
  GxCompStorage *store;
} GxEcs;

GxEcs *gx_ecs_new();
void gx_ecs_init(GxEcs *ecs, int numb_component, ...);
void gx_ecs_destroy(GxEcs *ecs);

GxEntity gx_ecs_create_entity(GxEcs *ecs, int numb_comp, ...);
void gx_ecs_delete_entity(GxEcs *ecs, GxEntity entt);

bool gx_ecs_insert_component(GxEcs *ecs, GxEntity entt, uint8_t comp_type,
                             const void *const comp);
void gx_ecs_delete_component(GxEcs *ecs, GxEntity entt, uint8_t comp_type);

GxQueryComp *gx_ecs_query(const GxEcs *const ecs, int n, ...);
GxQueryCompEntity *gx_ecs_query_entity(const GxEcs *const ecs, int n, ...);

#ifdef __cplusplus
}
#endif

#endif //__GX_ECS_H__
