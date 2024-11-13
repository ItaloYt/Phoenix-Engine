#version 450

vec2 vertexs[3] = {
  vec2(0.0, -0.5),
  vec2(0.5, 0.5),
  vec2(-0.5, 0.5),
};

void main() {
  gl_Position = vec4(vertexs[gl_VertexIndex], 0, 1);
}
