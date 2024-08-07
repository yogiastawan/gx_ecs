#ifndef __GX_ENTITY_H__
#define __GX_ENTITY_H__

#include "utils/gx_dllist.h"
#include "utils/gx_entity_queue.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef GX_DEF_ENTT_CAP
#define GX_DEF_ENTT_CAP 8
#endif

#ifndef GX_DEF_QUERY_CAP
#define GX_DEF_QUERY_CAP 8
#endif

#ifdef __cplusplus
exten "C" {
#endif

  typedef struct {
    size_t id;
  } GxEntity;

  typedef struct {
    uint64_t limb[4];
  } uint256_t;

  typedef struct {
    GxEntityQueue cache_entt;
    size_t numb_entt;
    uint8_t numb_comp;
    GxNode ***node_array_ptr;
    size_t cap;
    uint256_t *mask;
  } GxEntityCore;

  typedef struct {
    size_t size;
    size_t cap;
    void ***comp;
  } GxQueryComp;

  typedef struct {
    size_t size;
    size_t cap;
    GxEntity *entities;
    void ***comp;
  } GxQueryCompEntity;

  void gx_comp_mask_set_bit(uint256_t * mask, uint8_t b);
  bool gx_comp_mask_is_set(uint256_t mask, uint8_t b);
  bool gx_comp_mask_is_set_batch(uint256_t mask, uint8_t n, uint8_t * b);

  GxEntityCore gx_entity_core_new(uint8_t numb_comp);
  GxEntity gx_entity_core_insert(GxEntityCore * ec, uint8_t comp_type,
                                 GxNode * node);

  // TODO! move to ecs level
  // GxEntity gx_entity_core_insert_batch(GxEntityCore * ec, ...);

  bool gx_entity_core_add(GxEntityCore * ec, GxEntity entt, uint8_t comp_type,
                          GxNode * node);
  GxNode *gx_entity_core_remove_comp(GxEntityCore * ec, GxEntity entt,
                                     uint8_t comp_type);
  GxNode **gx_entity_core_remove_entity(GxEntityCore * ec, GxEntity entt);
  void gx_entity_core_destroy(GxEntityCore * ec);

  GxQueryComp *gx_query_comp_get(GxEntityCore * ec, int n, va_list args);
  void gx_query_comp_destroy(GxQueryComp * qc);
  GxQueryCompEntity *gx_query_comp_entity_get(GxEntityCore * ec, int n,
                                              va_list args);
  void gx_query_comp_entity_destroy(GxQueryCompEntity * qce);

#ifdef __cplusplus
}
#endif
#endif //__GX_ENTITY_H__
