#include "util/math.h"
#include <math.h>

void mat4_mul_vec4(float *result, float *a, float *b) {
  for(unsigned i = 0; i < 4; ++i) {
    result[i] = 0;

    for(unsigned ii = 0; ii < 4; ++ii) {
      result[i] += a[ii * 4 + i] * b[ii];
    }
  }
}

void mat4_mul_mat4(float *result, float *a, float *b) {
  for(unsigned i = 0; i < 4; ++i) {
    for(unsigned ii = 0; ii < 4; ++ii) {
      result[ii * 4 + i] = 0;

      for(unsigned iii = 0; iii < 4; ++iii) {
        result[ii * 4 + i] += a[iii * 4 + i] * b[ii * 4 + iii];
      }
    }
  }
}

void mat4_perspective(float *result, float aspect_ratio, float fov, float near, float far) {
  const float factor = tanf(fov * M_PI / 180.0);
  result[0] = 1 / (aspect_ratio * factor);
  result[1] = 0;
  result[2] = 0;
  result[3] = 0;
  result[4] = 0;
  result[5] = 1 / factor;
  result[6] = 0;
  result[7] = 0;
  result[8] = 0;
  result[9] = 0;
  result[10] = (-near - far) / (near - far);
  result[11] = 1;
  result[12] = 0;
  result[13] = 0;
  result[14] = 2 * far * near / (near - far);
  result[15] = 0;
}
