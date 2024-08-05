#include <assert.h>

#include "gx_comp.h"
#include "utils/gx_dllist.h"

typedef struct {
  float x;
  float y;
  float z;
} Position;

typedef struct {
  float x;
  float y;
  float z;
} Velocity;

enum Components { POSITION = 0, VELOCITY, NUMB_COMP };

int main() {

  Position pos = {0.0f, 0.3f, 0.1f};

  Velocity v = {0.1f, -0.1f, 0.0f};
  Velocity v2 = {0.2f, -0.2f, 0.0f};

  GxCompStorage gcs =
      gx_comp_storage_new(NUMB_COMP, make_comp(Position), make_comp(Velocity));

  GxNode *pn1 = gx_comp_storage_insert(&gcs, POSITION, &pos);
  GxNode *vn1 = gx_comp_storage_insert(&gcs, VELOCITY, &v);
  GxNode *vn2 = gx_comp_storage_insert(&gcs, VELOCITY, &v2);

  GxNode *vnd2 = gcs.comp[VELOCITY].node->next;

  assert(vnd2 != NULL);

  gx_comp_storage_remove(&gcs, VELOCITY, vn2);

  vnd2 = gcs.comp[VELOCITY].node->next;
  assert(vnd2 == NULL);

  gx_comp_destroy(&gcs);
  return 0;
}
