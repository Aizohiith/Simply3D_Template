#version 330

in vec4 vCol;
in vec2 TexCoord;
out vec4 colour;

uniform sampler2D theTexture;
uniform vec3 u_Color;

void main()
{
	colour = texture(theTexture, TexCoord) * vec4(u_Color, 1.0f);
}