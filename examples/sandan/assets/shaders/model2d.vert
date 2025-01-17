#version 410

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

uniform float rotation;
uniform vec2 scale;
uniform vec2 translation;

out vec4 fragColor;

void main() {
  float sinAngle = sin(rotation);
  float cosAngle = cos(rotation);
  vec2 rotated = vec2(inPosition.x * cosAngle - inPosition.y * sinAngle,
                      inPosition.x * sinAngle + inPosition.y * cosAngle);

  vec2 newPosition = vec2(rotated.x * scale.x, rotated.y * scale.y) + translation;
  gl_Position = vec4(newPosition, 0, 1);
  fragColor = inColor;
}
