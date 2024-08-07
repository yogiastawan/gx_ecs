#ifndef __GX_ENTITY_QUEUE_H__
#define __GX_ENTITY_QUEUE_H__

#include <stddef.h>

#ifdef __cplusplus
exten "C" {
#endif

  typedef struct _GxEntityNode GxEntityNode;

  typedef struct {
    size_t size;
    GxEntityNode *rear;
    GxEntityNode *peek;
    GxEntityNode *node;
  } GxEntityQueue;

  GxEntityQueue gx_entity_queue_new();
  void gx_entity_queue_enqueue(GxEntityQueue * eq, size_t val);
  size_t gx_entity_queue_dequeue(GxEntityQueue * eq);
  size_t gx_entity_queue_peek(const GxEntityQueue *const eq);
  size_t gx_entity_queue_rear(const GxEntityQueue *const eq);
  size_t gx_entity_queue_size(const GxEntityQueue *const eq);

  void gx_entity_queue_destroy(GxEntityQueue * eq);

#ifdef __cplusplus
}
#endif

#endif //__GX_ENTITY_QUEUE_H__
