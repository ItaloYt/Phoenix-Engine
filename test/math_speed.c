#include "util/math.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

mat4 m1 = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  10, 0, 0, 1
};

vec4 v1 = { 0, 0, -1, 0 };
vec4 vr;

int main() {
  clock_t start = clock();
  mat4_mul_vec4(vr, m1, v1);
  double time = (double) (clock() - start) / CLOCKS_PER_SEC;
  printf("Time: %.12f\n", time);
  printf("Result: [ %f, %f, %f, %f ]\n", vr[0], vr[1], vr[2], vr[3]);
}
