#ifndef PHOENIX_UTIL_MATHEMATICS_H
#define PHOENIX_UTIL_MATHEMATICS_H

// TODO: Use a library to do this, that's absolute shit

// typedef enum ElementCount {
//   ELEMENT_COUNT_2 = 0x21,
//   ELEMENT_COUNT_3 = 0x31,
//   ELEMENT_COUNT_4 = 0x41,
//   ELEMENT_COUNT_2X2 = 0x22,
//   ELEMENT_COUNT_2X3 = 0x23,
//   ELEMENT_COUNT_2X4 = 0x24,
//   ELEMENT_COUNT_3X2 = 0x32,
//   ELEMENT_COUNT_3X3 = 0x33,
//   ELEMENT_COUNT_3X4 = 0x34,
//   ELEMENT_COUNT_4X2 = 0x42,
//   ELEMENT_COUNT_4X3 = 0x43,
//   ELEMENT_COUNT_4X4 = 0x44,
// } ElementCount;

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

typedef float mat4[16];
typedef float mat4x4[4][4];

typedef struct point2 {
  float x, y;
} point2;

typedef struct point3 {
  float x, y, z;
} point3;

static const mat4 MAT4_IDENTITY = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};

static const mat4 MAT4_ZERO = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};

void mat4_mul_vec4(float *result, float *a, float *b);
void mat4_mul_mat4(float *result, float *a, float *b);
void mat4_perspective(float *result, float aspect_ratio, float fov, float near, float far);

#endif
