#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out vec3 outPosition;
out vec2 outTexCoord;
out float stp;

uniform float factor;


void main() {

  gl_Position = vec4(Position, 1.0f);

  outTexCoord = TexCoords;
}