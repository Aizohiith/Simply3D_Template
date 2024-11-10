#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <algorithm>


AmbientLight::AmbientLight()
{
	Intencity = 1.0f;
	Color = glm::vec3(1.0f);

}

void AmbientLight::SetIntencity(float pf_NewIntencity)
{
	Intencity = pf_NewIntencity;
}

float AmbientLight::GetIntencity() const
{
	return Intencity;
}

void AmbientLight::SetColor(float pf_R, float pf_G, float pf_B)
{
	Color.x = pf_R;
	Color.y = pf_G;
	Color.z = pf_B;
}
void AmbientLight::SetColor(glm::vec3 newColor)
{
	Color.x = newColor.x;
	Color.y = newColor.y;
	Color.z = newColor.z;
}
glm::vec3 AmbientLight::GetColor() const
{
	return Color;
}


void AmbientLight::Use(GLuint pi_IntencityID, GLuint pi_ColorID)
{
	glUniform3f(pi_ColorID, Color.x, Color.y, Color.z);
	glUniform1f(pi_IntencityID, Intencity);
}

void DirectionalLight::SetShadowMap(Shadow& NewShadowMap)
{
	ShadowMap = &NewShadowMap;
}
Shadow* DirectionalLight::GetShadowMap() const
{
	return ShadowMap;
}
DirectionalLight::~DirectionalLight()
{
	if (ShadowMap != nullptr)
		delete ShadowMap;
	ShadowMap = nullptr;
}

//glm::mat4 DirectionalLight::GetTransform()
//{
//	return Projection * glm::lookAt(Direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0));
//}

glm::mat4 DirectionalLight::GetTransform(const glm::mat4& Project, const glm::mat4& cameraView) const
{
	const auto inv = glm::inverse(Project * cameraView);



	std::array<glm::vec4, 8> frustumCorners = {
		glm::vec4(-1, -1, -1, 1), glm::vec4(1, -1, -1, 1),
		glm::vec4(1, 1, -1, 1), glm::vec4(-1, 1, -1, 1),
		glm::vec4(-1, -1, 1, 1), glm::vec4(1, -1, 1, 1),
		glm::vec4(1, 1, 1, 1), glm::vec4(-1, 1, 1, 1)
	};

	for (auto& corner : frustumCorners) {
		corner = inv * corner;
		corner /= corner.w;
	}

	glm::vec3 frustumCenter = glm::vec3(0.0f);
	for (const auto& corner : frustumCorners) {
		frustumCenter += glm::vec3(corner);
	}
	frustumCenter /= frustumCorners.size();

	auto lightView = glm::lookAt(frustumCenter + glm::normalize(Direction), frustumCenter, glm::vec3(0.0f, 1.0f, 0.0f));

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const auto& corner : frustumCorners) {
		const auto trf = lightView * corner;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	float shadowNearPlane = 0.1f;  // Near plane of shadow map (depending on your scene setup)
	float shadowFarPlane = 100.0f; // Far plane of shadow map (limit to a reasonable distance)

	// Use these values when computing the light's projection matrix
	glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

	//glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

	return lightProjection * lightView;
}

DirectionalLight::DirectionalLight()
{
	ShadowMap = nullptr;
	gf_Render_Distance = 50.0f;
	SetShadowBits(12);
}

void DirectionalLight::Use(GLuint pi_IntencityID, GLuint pi_ColorID, GLuint pi_DirectionID)
{
	glUniform3f(pi_ColorID, Color.x, Color.y, Color.z);
	glUniform1f(pi_IntencityID, Intencity);
	glUniform3f(pi_DirectionID, Direction.x, Direction.y, Direction.z);

}

void DirectionalLight::SetDirection(float pf_x, float pf_y, float pf_z)
{
	Direction.x = pf_x;
	Direction.y = pf_y;
	Direction.z = pf_z;
}

void DirectionalLight::SetDirection(glm::vec3 NewDirection)
{
	Direction = -NewDirection;
}
glm::vec3 DirectionalLight::GetDirection() const
{
	return -Direction;
}

void DirectionalLight::SetShadowBits(float pf_Bits)
{
	gf_ShadowBits = pf_Bits;
	if (ShadowMap != nullptr)
	{
		delete ShadowMap;
		ShadowMap = nullptr;
	}

	ShadowMap = new Shadow();
	ShadowMap->Init(static_cast<int>(pow(2, gf_ShadowBits)), static_cast<int>(pow(2, gf_ShadowBits)));
}
float DirectionalLight::GetShadowBits() const
{
	return gf_ShadowBits;
}

void DirectionalLight::SetShadowRenderDistance(float pf_Distance)
{
	gf_Render_Distance = pf_Distance;
}
float DirectionalLight::GetShadowRenderDistance() const
{
	return gf_Render_Distance;
}

PointLight::PointLight()
{
	Color = glm::vec3(1.0f);
	Position = glm::vec3(0.0f);
	Intencity = 1.0f;
	AttenuationConst = glm::vec3(0.1f, 0.2f, 1.0f);
}

void PointLight::SetRange(float Range)
{
	AttenuationConst.x = 0.1f / ( Range * Range);
	AttenuationConst.y = 0.2f / Range;
}


void PointLight::Use(GLuint pi_IntencityID, GLuint pi_ColorID, GLuint pi_PositionID, GLuint pi_AttenuationID)
{
	glUniform3f(pi_ColorID, Color.x, Color.y, Color.z);
	glUniform1f(pi_IntencityID, Intencity);
	glUniform3f(pi_PositionID, Position.x, Position.y, Position.z);
	glUniform3f(pi_AttenuationID, AttenuationConst.x, AttenuationConst.y, AttenuationConst.z);

}

void PointLight::SetPosition(float pf_x, float pf_y, float pf_z)
{
	Position.x = pf_x;
	Position.y = pf_y;
	Position.z = pf_z;
}

void PointLight::SetPosition(glm::vec3 NewPosition)
{
	Position = NewPosition;
}
glm::vec3 PointLight::GetPosition() const
{
	return Position;
}

void PointLight::SetAttenuationFormula(float pf_a, float pf_b, float pf_c)
{
	AttenuationConst.x = pf_a;
	AttenuationConst.y = pf_b;
	AttenuationConst.z = pf_c;
}

void PointLight::SetAttenuationFormula(glm::vec3 NewConstants)
{
	AttenuationConst = NewConstants;
}

glm::vec3 PointLight::GetAttenuationFormula() const
{
	return AttenuationConst;
}

