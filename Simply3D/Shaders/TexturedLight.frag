#version 330


in vec4 vCol;
in vec2 TexCoord;
out vec4 colour;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightPos;

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

struct PointLight
{
	float Intencity;
	vec3 color;
	vec3 Position;
	vec3 AttenuationFormula;
};

struct Material
{
	float Intencity;
	float Shiness;
};


uniform vec3 u_CameraPos;
uniform Material u_mat;

uniform sampler2D theTexture;
uniform sampler2D DirectionalShadowMap;

uniform AmbientLight ALight;
uniform DirectionalLight DLight;
uniform PointLight PLight[100];
uniform vec3 u_Color;
uniform int LightCount;

vec2 randomOffset(vec2 coord)
{
	float randomValue = fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
	return vec2(randomValue, fract(randomValue * 3.14));
}

float ShadowFactor()
{
	vec3 projCoords = DirectionalLightPos.xyz / DirectionalLightPos.w;
	projCoords = (projCoords * 0.5) + 0.5;
	
	float current = projCoords.z;

	if(projCoords.z > 1.0)
		return 0.0;							
	
	
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(DLight.Direction);
	
	float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(DirectionalShadowMap, 0);
	float totalWeight = 0.0;
	
	
	

	// Function to generate a pseudo-random vector based on fragment coordinates

	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			vec2 offset = (vec2(x, y) + randomOffset(projCoords.xy + vec2(x, y))) * texelSize;
			float pcfDepth = texture(DirectionalShadowMap, projCoords.xy + offset).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
			totalWeight += 1.0;
		}
	}

	// Average the result to get the shadow factor
	shadow /= totalWeight;
	
	return shadow;
}

vec4 CalculateDirectionalLight(vec3 pv_Direction, vec3  pv_Color, float pf_Intencity, float pf_Shadow)
{
	float DotProduct = max(dot(normalize(Normal), normalize(pv_Direction)), 0.0f);
	vec4 DColor = vec4(pv_Color, 1.0f) * pf_Intencity * DotProduct;


	vec4 SColor = vec4(0, 0, 0, 0);
	
	if(DotProduct > 0.0f)
	{
		vec3 fragToEye = normalize(FragPos - u_CameraPos);
		vec3 reflectedVertex = normalize(reflect(pv_Direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, u_mat.Shiness);
			SColor = vec4(pf_Intencity  * pv_Color * u_mat.Intencity * specularFactor, 1.0f);
		}
	}

	return (1 - pf_Shadow) * (DColor + SColor);
}

vec4 CalculatePointLight()
{
	vec4 OutColor = vec4(0.0f);

	for (int C1 = 0; C1 < LightCount; C1++)
	{
		vec3 Dir = PLight[C1].Position - FragPos;
		float Dist = length(Dir);
		Dir = normalize(Dir);

		float Attenuation = (Dist * Dist * PLight[C1].AttenuationFormula.x + Dist * PLight[C1].AttenuationFormula.y + PLight[C1].AttenuationFormula.z);

		OutColor += CalculateDirectionalLight(Dir,PLight[C1].color,PLight[C1].Intencity, 0.0f) / Attenuation;
	}

	return OutColor;
}

void main()
{
	vec4 AColor = vec4(ALight.color, 1.0f) * ALight.Intencity;//Ambient Color
	vec4 DColor = CalculateDirectionalLight(DLight.Direction, DLight.color, DLight.Intencity, ShadowFactor());//Directional Color
	vec4 PColor = CalculatePointLight();//Point Color
	colour = texture(theTexture, TexCoord) * (AColor + DColor + PColor) * vec4(u_Color, 1.0f);//Final Color
}