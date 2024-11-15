#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

int vec3 Color
out vec4 vCol;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(Color, 1.0f);
	
	TexCoord = tex;
	Normal =  mat3(transpose(inverse(model))) * norm;
}