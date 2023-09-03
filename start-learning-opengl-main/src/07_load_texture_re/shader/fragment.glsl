#version 330 core
out vec4 FracColor;
in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main(){
  FracColor = texture(texture1, TexCoord);
}