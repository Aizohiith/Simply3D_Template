#pragma once
//Done
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 StartPosition, glm::vec3 StartRotation, glm::vec3 WorldUp);

	void Update();//Update Camera

	void SetPosition(glm::vec3 NewPosition);//Set Position
	void SetPosition(float x, float y, float z);//Set Position
	void SetRotation(glm::vec3 NewRotation);//Set Rotation
	void SetRotation(float x, float y, float z);//Set Rotation
	void Translate(glm::vec3 MoveBy);
	void Translate(float x, float y, float z);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 gg_Front;
	glm::vec3 gg_Position;
	glm::vec3 gg_Up;
	glm::vec3 gg_Right;
	glm::vec3 gg_WorldUp;
	glm::vec3 gg_Rotation;
};