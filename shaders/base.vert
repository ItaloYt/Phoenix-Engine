#version 450

layout(location = 0) in vec2 in_vertex;
layout(set = 0, binding = 0) uniform ObjectData {
    mat4 projection;
} data;

void main() {
    gl_Position = data.projection * vec4(in_vertex, 1, 1);
}
