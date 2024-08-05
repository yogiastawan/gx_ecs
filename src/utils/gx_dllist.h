#ifndef __GX_DLLIST_H__
#define __GX_DLLIST_H__

#include <stddef.h>

#ifdef __cpluplus
extern "C" {
#endif

struct _GxNode {
  void *data;
  struct _GxNode *next;
  struct _GxNode *prev;
};

typedef struct _GxNode GxNode;

typedef struct {
  size_t size_bytes;
  GxNode *node;
  GxNode *last_node;
} GxDLList;

GxDLList gx_dllist_new(size_t size_bytes);
void gx_dllist_append(GxDLList *ll, const void *const data);
void gx_dllist_delete(GxDLList *ll, const void *const data);
void gx_dllist_destroy(GxDLList *ll);

#ifdef __cplusplus
}
#endif

#endif //__GX_DLLIST_H__
