#include "gx_dllist.h"
#include <stdlib.h>
#include <string.h>

GxDLList gx_dllist_new(size_t size_bytes) {
  GxDLList ll = {.size_bytes = size_bytes, .node = NULL, .last_node = NULL};
  return ll;
}

void gx_dllist_append(GxDLList *ll, const void *const data) {
  size_t size = ll->size_bytes;

  GxNode *node = malloc(sizeof(GxNode));
  node->data = malloc(size);
  node->next = NULL;
  node->prev = NULL;
  memcpy(node->data, data, size);

  if (!ll->last_node) {
    ll->node = node;
    ll->last_node = node;
    return;
  }
  node->prev = ll->last_node;
  ll->last_node->next = node;
  ll->last_node = node;
}

void gx_dllist_delete(GxDLList *ll, const void *const at_data) {
  GxNode *node = (GxNode *)at_data;
  if (ll->last_node == node) {
    ll->last_node = node->prev;
    free(node);
    ll->last_node->next = NULL;
    return;
  }

  node->prev->next = node->next;
  node->next->prev = node->prev;
  free(node);
}

void gx_dllist_destroy(GxDLList *ll) {
  GxNode *node = ll->node;
  while (node) {
    GxNode *tmp = node->next;
    free(node);
    node = tmp;
  }
  ll->node = NULL;
  ll->last_node = NULL;
}
