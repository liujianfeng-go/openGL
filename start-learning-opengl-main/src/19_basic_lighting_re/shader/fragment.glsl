#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
uniform vec3 lightColor;
uniform float ambientStrength;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  // FragColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  vec3 ambient = ambientStrength * lightColor;
  vec3 result = ambient * texture(texture1, outTexCoord).rgb;
  FragColor = vec4(result, 1.0);
}