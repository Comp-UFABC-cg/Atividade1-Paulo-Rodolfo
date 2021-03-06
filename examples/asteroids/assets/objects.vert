#version 410

layout(location = 0) in vec2 inPosition;


uniform vec4 color;
uniform float rotation;
uniform float scale;
uniform vec2 translation;
uniform vec2 navePosition;

out vec4 fragColor;

void main() {
  

  vec2 rotated = vec2((inPosition.x + rotation),
                      inPosition.y);
  
  vec2 newPosition = (rotated * scale) + translation;
  gl_Position = vec4(newPosition, 0, 1);
  fragColor = color;
}