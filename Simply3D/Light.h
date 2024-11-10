#pragma once

#include "CommonValues.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "Shadow.h"

class AmbientLight
{
	public:
		AmbientLight();

		void SetIntencity(float NewIntencity);
		float GetIntencity() const;

		void SetColor(float R, float G, float B);
		void SetColor(glm::vec3 newColor);
		glm::vec3 GetColor() const;

		void Use(GLuint pf_IntencityID, GLuint pf_ColorID);

	protected:
		float Intencity;
		glm::vec3 Color;
};

class  DirectionalLight : public AmbientLight
{
public:
	DirectionalLight();
	void Use(GLuint pi_IntencityID, GLuint pi_ColorID, GLuint pi_DirectionID);
	void SetDirection(float x, float y, float z);
	void SetDirection(glm::vec3 NewDirection);

	glm::mat4 GetTransform(const glm::mat4& Project,const glm::mat4& cameraView) const;
	glm::vec3 GetDirection() const;

	void SetShadowBits(float pf_Bits);
	float GetShadowBits() const;
	void SetShadowRenderDistance(float pf_Distance);
	float GetShadowRenderDistance() const;
	void SetShadowMap(Shadow& NewShadowMap);


	Shadow* GetShadowMap() const;

	~DirectionalLight();

private:
	float gf_ShadowBits;
	float gf_Render_Distance;
	glm::vec3 Direction;
	Shadow* ShadowMap;

};

class PointLight : public AmbientLight
{
	public:
		PointLight();
		void Use(GLuint IntencityID, GLuint ColorID, GLuint PositionID, GLuint AttenuationID);
		
		void SetPosition(float x, float y, float z);
		void SetPosition(glm::vec3 NewPosition);
		glm::vec3 GetPosition() const;

		void SetAttenuationFormula(float a, float b, float c);
		void SetAttenuationFormula(glm::vec3 NewConstants);
		glm::vec3 GetAttenuationFormula() const;

		void SetRange(float Range);

	private:
		glm::vec3 Position;
		glm::vec3 AttenuationConst; //a^2x + bx + c. x = a, y = b, z = c
};

