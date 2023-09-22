#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 ourLightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
}