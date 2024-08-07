#include "gx_entity_queue.h"

#include <stdlib.h>

#include "gx_entity.h"
#include "utils/gx_deb.h"

#ifdef __cplusplus
exten "C" {
#endif

  struct _GxEntityNode {
    GxEntity val;
    struct _GxEntityNode *next;
    struct _GxEntityNode *prev;
  };

#ifdef __cplusplus
}
#endif

GxEntityQueue gx_entity_queue_new() {
  GxEntityQueue eq = {
      .size = 0, .node = malloc(sizeof(GxEntityNode)), .rear = NULL};
  eq.node->next = NULL;
  eq.node->prev = NULL;
  eq.rear = eq.node;

  return eq;
}

void gx_entity_queue_enqueue(GxEntityQueue *eq, size_t val) {
  gx_assert(eq == NULL, "GxEntityQueue must be not NULL");

  if (eq->size <= 0) {
    eq->node->val = (GxEntity){val};
    eq->size++;
    return;
  }

  GxEntityNode *node = malloc(sizeof(GxEntityNode));
  node->val = (GxEntity){val};
  node->prev = NULL;

  node->next = eq->node;
  eq->node->prev = node;

  eq->node = node;
  eq->size++;
}

size_t gx_entity_queue_dequeue(GxEntityQueue *eq) {
  gx_assert(eq == NULL, "GxEntityQueue must be not NULL");

  gx_assert(eq->size <= 0, "Cannot dequeue when size is %lu", eq->size);

  GxEntity entt = eq->rear->val;

  eq->rear = eq->rear->prev;

  free(eq->rear->next);

  eq->rear->next = NULL;

  eq->size--;

  return entt.id;
}

size_t gx_entity_queue_peek(GxEntityQueue *eq) {
  gx_assert(eq == NULL, "GxEntityQueue must be not NULL");
  return eq->node->val.id;
}

size_t gx_entity_queue_rear(GxEntityQueue *eq) {
  gx_assert(eq == NULL, "GxEntityQueue must be not NULL");
  return eq->rear->val.id;
}

size_t gx_entity_queue_size(GxEntityQueue *eq) {
  gx_assert(eq == NULL, "GxEntityQueue must be not NULL");
  return eq->size;
}

void gx_entity_queue_destroy(GxEntityQueue *eq) {
  gx_assert(eq == NULL, "GxEntityQueue must be not NULL");

  GxEntityNode *node = eq->node;

  while (node) {
    GxEntityNode *tmp = node->next;
    free(node);
    node = tmp;
  }
  eq->node = NULL;
  eq->rear = NULL;
}
