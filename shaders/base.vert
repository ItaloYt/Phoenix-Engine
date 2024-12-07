#version 450

layout(location = 0) in vec2 in_vertex;

void main() {
  gl_Position = vec4(in_vertex, 0, 1);
}
