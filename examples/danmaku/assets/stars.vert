#version 410

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in float inSize;

out vec4 fragColor;

void main() {
  gl_PointSize = inSize;
  gl_Position = vec4(inPosition.xy, 0, 1);
  fragColor = vec4(inColor, 1);
}