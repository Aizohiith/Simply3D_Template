#version 330

layout (location = 0) in vec3 pos;

out vec4 vCol;

uniform mat4 model;
uniform mat4 projectionview;

void main()
{
	gl_Position = projectionview * model * vec4(pos, 1.0);
}