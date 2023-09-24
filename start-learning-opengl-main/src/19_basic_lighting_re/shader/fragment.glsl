#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStrength;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  vec4 objectColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  vec3 ambient = ambientStrength * lightColor;  // 环境光

  // 计算漫反射光照
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;  // 漫反射光

  vec3 result = (ambient + diffuse) * objectColor.rgb;
  FragColor = vec4(result, 1.0);
}