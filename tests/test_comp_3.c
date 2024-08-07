#include <assert.h>

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

GxCompStorage create_comp(int n, ...) {
  va_list args;
  va_start(args, n);
  GxCompStorage a = gx_comp_storage_new(n, args);
  va_end(args);
  return a;
}

int main() {

  Position pos = {0.0f, 0.3f, 0.1f};

  Velocity v = {0.1f, -0.1f, 0.0f};
  Velocity v2 = {0.2f, -0.2f, 0.0f};
  Velocity v3 = {0.3f, -0.1f, 0.5f};

  GxCompStorage gcs =
      create_comp(NUMB_COMP, make_comp(Position), make_comp(Velocity));

  Position *gcs_pos = gx_comp_storage_insert(&gcs, POSITION, &pos)->data;
  Velocity *gcs_v = gx_comp_storage_insert(&gcs, VELOCITY, &v)->data;
  Velocity *gcs_v2 = gx_comp_storage_insert(&gcs, VELOCITY, &v2)->data;
  Velocity *gcs_v3 = gx_comp_storage_insert(&gcs, VELOCITY, &v3)->data;
  assert(gcs_pos != NULL);
  assert(gcs_v != NULL);
  assert(gcs_v2 != NULL);
  assert(gcs_v3 != NULL);

  assert(pos.x == gcs_pos->x);
  assert(pos.y == gcs_pos->y);
  assert(pos.z == gcs_pos->z);

  assert(v2.x == gcs_v2->x);
  assert(v2.y == gcs_v2->y);
  assert(v2.z == gcs_v2->z);

  assert(v3.x == gcs_v3->x);
  assert(v3.y == gcs_v3->y);
  assert(v3.z == gcs_v3->z);

  gx_comp_storage_destroy(&gcs);
  return 0;
}
