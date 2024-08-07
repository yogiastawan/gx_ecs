#include "gx_entity.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "utils/gx_deb.h"
#include "utils/gx_dllist.h"
#include "utils/gx_entity_queue.h"

void gx_comp_mask_set_bit(uint256_t *mask, uint8_t b) {
  uint8_t shift = b % 64;
  int index = b / 64;
  gx_assert(index > 3, "Index is %i, but max limb is 4", index);
  uint64_t x = 1 << shift;
  mask->limb[index] |= x;
}

bool gx_comp_mask_is_set(uint256_t mask, uint8_t b) {
  uint8_t shift = b % 64;
  int index = b / 64;
  gx_assert(index > 3, "Index is %i, but max limb is 4", index);
  uint64_t x = 1 << shift;
  return mask.limb[index] & x;
}

bool gx_comp_mask_is_set_batch(uint256_t mask, uint8_t n, uint8_t *b) {

  for (uint8_t i = 0; i < n; i++) {
    if (!gx_comp_mask_is_set(mask, b[i])) {
      return false;
    }
  }
  return true;
}

GxEntityCore gx_entity_core_new(uint8_t numb_comp) {
  GxEntityCore ec = {.cache_entt = gx_entity_queue_new(),
                     .numb_entt = 0,
                     .numb_comp = numb_comp,
                     .node_array_ptr = malloc(sizeof(GxNode *) * numb_comp),
                     .cap = GX_DEF_ENTT_CAP,
                     .mask = calloc(GX_DEF_ENTT_CAP, sizeof(uint256_t))};

  for (uint8_t i = 0; i < numb_comp; i++) {
    ec.node_array_ptr[i] = malloc(sizeof(GxNode *) * GX_DEF_ENTT_CAP);
  }

  return ec;
}

GxEntity gx_entity_core_insert(GxEntityCore *ec, uint8_t comp_type,
                               GxNode *node) {
  gx_assert(comp_type >= ec->numb_comp,
            "Access of component %u, but number component is %u ", comp_type,
            ec->numb_comp);

  gx_assert(node == NULL, "Node must be not null");

  // insert to cache if there is cached.
  if (gx_entity_queue_size(&ec->cache_entt) >= 0) {
    size_t entt = gx_entity_queue_dequeue(&ec->cache_entt);
    gx_comp_mask_set_bit(&ec->mask[entt], comp_type);
    ec->node_array_ptr[comp_type][entt] = node;
    return (GxEntity){entt};
  }

  size_t entt = ec->numb_entt;
  if (entt >= ec->cap) {
    ec->cap *= 2;
    for (uint8_t i = 0; i < ec->numb_comp; i++) {
      ec->node_array_ptr[i] =
          realloc(ec->node_array_ptr[i], sizeof(GxNode *) * ec->cap);
    }
    ec->mask = realloc(ec->mask, sizeof(uint256_t) * ec->cap);
  }

  gx_comp_mask_set_bit(&ec->mask[entt], comp_type);
  ec->node_array_ptr[comp_type][entt] = node;
  ec->numb_entt++;
  return (GxEntity){entt};
}

bool gx_entity_core_add(GxEntityCore *ec, GxEntity entt, uint8_t comp_type,
                        GxNode *node) {
  gx_assert(
      entt.id >= ec->numb_entt,
      "Adding component to entit id: %lu, but current max entity id is %lu",
      entt.id, ec->numb_entt);
  gx_assert(comp_type >= ec->numb_comp,
            "Access of component %u, but number component is %u", comp_type,
            ec->numb_comp);
  gx_assert(node == NULL, "Node must be not null");

  if (gx_comp_mask_is_set(*ec->mask, comp_type)) {
    return false;
  }

  ec->node_array_ptr[comp_type][entt.id] = node;

  return true;
}

GxNode *gx_entity_core_remove_comp(GxEntityCore *ec, GxEntity entt,
                                   uint8_t comp_type) {
  gx_assert(entt.id >= ec->numb_entt,
            "Removing component in entity id: %lu, but current max entity id "
            "is %lu",
            entt.id, ec->numb_entt);
  gx_assert(comp_type >= ec->numb_comp,
            "Access of component %u, but number component is %u", comp_type,
            ec->numb_comp);

  GxNode *tmp = ec->node_array_ptr[comp_type][entt.id];
  ec->node_array_ptr[comp_type][entt.id] = NULL;
  gx_entity_queue_enqueue(&ec->cache_entt, entt.id);

  return tmp;
}

GxNode **gx_entity_core_remove_entity(GxEntityCore *ec, GxEntity entt) {
  gx_assert(entt.id >= ec->numb_entt,
            "Cannot Access entity id: %lu when max entity is %lu", entt.id,
            ec->numb_entt);
  GxNode **nodes = malloc(sizeof(GxNode *) * ec->numb_comp);
  for (uint8_t i = 0; i < ec->numb_comp; i++) {
    nodes[i] = ec->node_array_ptr[i][entt.id];
    ec->node_array_ptr[i][entt.id] = NULL;
  }

  return nodes;
}

void gx_entity_core_destroy(GxEntityCore *ec) {
  gx_assert(!ec, "Cannot free of NULL GxEntityCore");

  gx_entity_queue_destroy(&ec->cache_entt);

  free(ec->mask);
  ec->mask = NULL;
  for (uint8_t i = 0; i < ec->numb_comp; i++) {
    free(ec->node_array_ptr[i]);
  }
  free(ec->node_array_ptr);
  ec->node_array_ptr = NULL;
}

GxQueryComp *gx_query_comp_get(GxEntityCore *ec, int n, va_list args) {
  gx_assert(ec == NULL, "Cannot access of NULL GxEntityCore");

  GxQueryComp *qc = malloc(sizeof(GxQueryComp));
  qc->comp = malloc(sizeof(void *) * n);
  qc->cap = GX_DEF_QUERY_CAP;

  uint8_t *comp = malloc(sizeof(uint8_t) * n);

  for (uint8_t i = 0; i < n; i++) {
    int b = va_arg(args, int);
    comp[i] = b;
    qc->comp = malloc(sizeof(void *) * GX_DEF_QUERY_CAP);
  }

  for (size_t i = 0; i < ec->numb_entt; i++) {
  _parent:
    if (qc->size >= qc->cap) {
      qc->cap *= 2;
      for (uint8_t j = 0; j < n; j++) {
        qc->comp[j] = realloc(qc->comp, sizeof(void *) * qc->cap);
      }
    }

    for (uint8_t j = 0; j < n; j++) {
      if (!gx_comp_mask_is_set(ec->mask[i], comp[j])) {
        i++;
        goto _parent;
      }
      qc->comp[qc->size][j] = ec->node_array_ptr[i][j];
    }
    qc->size++;
  }
  return qc;
}

void gx_query_comp_destroy(GxQueryComp *qc) {
  gx_assert(qc == NULL, "Cannot destroy of NULL GxQueryComp");
  for (uint8_t i = 0; i < qc->size; i++) {
    free(qc->comp[i]);
  }
  free(qc->comp);
  qc->comp = NULL;
}

GxQueryCompEntity *gx_query_comp_entity_get(GxEntityCore *ec, int n,
                                            va_list args) {
  gx_assert(ec == NULL, "Cannot access of NULL GxEntityCore");

  GxQueryCompEntity *qce = malloc(sizeof(GxQueryCompEntity));
  qce->comp = malloc(sizeof(void *) * n);
  qce->entities = malloc(sizeof(GxEntity) * GX_DEF_QUERY_CAP);
  qce->cap = GX_DEF_QUERY_CAP;

  uint8_t *comp = malloc(sizeof(uint8_t) * n);

  for (uint8_t i = 0; i < n; i++) {
    int b = va_arg(args, int);
    comp[i] = b;
    qce->comp = malloc(sizeof(void *) * GX_DEF_QUERY_CAP);
  }

  for (size_t i = 0; i < ec->numb_entt; i++) {
  _parent:
    if (qce->size >= qce->cap) {
      qce->cap *= 2;
      qce->entities = realloc(qce->entities, sizeof(GxEntity) * qce->cap);
      for (uint8_t j = 0; j < n; j++) {
        qce->comp[j] = realloc(qce->comp, sizeof(void *) * qce->cap);
      }
    }

    for (uint8_t j = 0; j < n; j++) {
      if (!gx_comp_mask_is_set(ec->mask[i], comp[j])) {
        i++;
        goto _parent;
      }
      qce->comp[qce->size][j] = ec->node_array_ptr[i][j];
    }
    qce->entities[qce->size] = (GxEntity){i};
    qce->size++;
  }
  return qce;
}

void gx_query_comp_entity_destroy(GxQueryCompEntity *qce) {
  gx_assert(qce == NULL, "Cannot destroy of NULL GxQueryCompEntity");
  free(qce->entities);
  qce->entities = NULL;
  for (uint8_t i = 0; i < qce->size; i++) {
    free(qce->comp[i]);
  }
  free(qce->comp);
  qce->comp = NULL;
}
