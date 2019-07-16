#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvs;
layout (location = 2) in vec3 normal;

layout (location = 0) uniform mat4 VP;
layout (location = 1) uniform mat4 TG;

out vec2 FragUvs;

void main() {
  FragUvs = uvs;
  gl_Position = VP * TG * vec4(position, 1);
}