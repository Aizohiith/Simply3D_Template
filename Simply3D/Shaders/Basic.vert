#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 u_Color;
void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(u_Color, 1.0f);
}