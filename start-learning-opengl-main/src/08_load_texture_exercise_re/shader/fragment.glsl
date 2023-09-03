#version 330 core
out vec4 FracColor;
in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
  FracColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x, TexCoord.y)), 0.5);
}