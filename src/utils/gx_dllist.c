#include "gx_dllist.h"
#include <stdlib.h>
#include <string.h>

GxLList gx_llist_new(size_t size_bytes) {
  GxLList ll = {.size_bytes = size_bytes, .node = NULL, .last_node = NULL};
  return ll;
}

void gx_llist_append(GxLList *ll, const void *data) {
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

void gx_llist_delete(GxLList *ll, const void *const at_data) {
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

void gx_llist_destroy(GxLList *ll) {
  GxNode *node = ll->node;
  while (node) {
    free(node);
    node = node->next;
  }
  ll->node = NULL;
  ll->last_node = NULL;
}
