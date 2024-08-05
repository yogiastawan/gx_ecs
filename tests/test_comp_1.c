#include <assert.h>
#include <stdio.h>

#include "gx_comp.h"

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

  assert(gx_comp_storage_insert(&gcs, POSITION, &pos) != NULL);
  assert(gx_comp_storage_insert(&gcs, VELOCITY, &v) != NULL);
  assert(gx_comp_storage_insert(&gcs, VELOCITY, &v2) != NULL);

  Position gcs_pos = *(Position *)gcs.comp[POSITION]
                          .node->data; //((Position **)gcs.comp)[POSITION][0];
  printf(">>Pos: %f, %f, %f\n", gcs_pos.x, gcs_pos.y, gcs_pos.z);
  assert(pos.x == gcs_pos.x);
  assert(pos.y == gcs_pos.y);
  assert(pos.z == gcs_pos.z);

  Velocity gcs_v2 = *(Velocity *)gcs.comp[VELOCITY].node->next->data;
  printf(">>Vel: %f, %f, %f\n", gcs_v2.x, gcs_v2.y, gcs_v2.z);
  assert(v2.x == gcs_v2.x);
  assert(v2.y == gcs_v2.y);
  assert(v2.z == gcs_v2.z);

  gx_comp_destroy(&gcs);
  return 0;
}
