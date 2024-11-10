#version 330

in vec4 vCol;
out vec4 colour;
in vec3 Normal;
struct AmbientLight
{
	float Intencity;
	vec3 color;
};
struct DirectionalLight
{
	float Intencity;
	vec3 color;
	vec3 Direction;
};

uniform AmbientLight ALight;
uniform DirectionalLight DLight;

void main()
{
	float DotProduct = max(dot(normalize(Normal), normalize(DLight.Direction)), 0.0f);
	
	vec4 AColor = vec4(ALight.color, 1.0f) * ALight.Intencity;
	vec4 DColor = vec4(DLight.color, 1.0f) * DLight.Intencity * DotProduct;
	colour = Color* (AColor + DColor);
}