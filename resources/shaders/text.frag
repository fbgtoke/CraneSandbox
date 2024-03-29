#version 430 core

uniform sampler2D tex;

in vec2 FragUvs;
out vec4 FragColor;

void main() {
  //FragColor = vec4(0.0, 1.0, 0.0, 1.0) + 0.0001 * vec4(1.0, 1.0, 1.0, texture(tex, FragUvs).r);
  //FragColor = vec4(FragUvs.x, FragUvs.y, 0.0, 1.0) + 0.0001 * vec4(1.0, 1.0, 1.0, texture(tex, FragUvs).r);
  FragColor = vec4(1.0, 1.0, 1.0, 1.0) * vec4(1.0, 1.0, 1.0, texture(tex, FragUvs).r);
}