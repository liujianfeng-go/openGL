#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float specularStrength; 

uniform sampler2D texture1;
uniform sampler2D texture2;

// 定义材质结构体
struct Material {
    vec3 ambient;  // 环境光 顔色
    vec3 diffuse;  // 漫反射光 顔色
    vec3 specular;  // 高光顔色
    float shininess;  // 高光系数
};
uniform Material material;

// 光的属性的结构体
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main() {
  vec4 objectColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);

  // 计算环境光照
  vec3 ambient = light.ambient * material.ambient;  // 环境光

  // 计算漫反射光照
  vec3 norm = normalize(outNormal);  // 法向量
  vec3 lightDir = normalize(lightPos - outFragPos);  // 光线
  float diff = max(dot(norm, lightDir), 0.0);  // 漫反射光 = max(法向量·光线, 0)
  vec3 diffuse = light.diffuse * diff * material.diffuse;  // 漫反射光

  // 计算镜面光照
  vec3 viewDir = normalize(viewPos - outFragPos); // 视线
  vec3 reflectDir = reflect(-lightDir, norm); // 反射光
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 镜面光 = pow(max(反射光·视线, 0), 光泽度)
  vec3 specular = light.specular * spec * material.specular; // 镜面光

  vec3 result = (ambient + diffuse + specular) * objectColor.rgb;
  FragColor = vec4(result, 1.0);
}