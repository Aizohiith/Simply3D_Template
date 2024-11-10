#include "Camera.h"

Camera::Camera()//There
{
	gg_Front = glm::vec3(0.0f);
	gg_Position = glm::vec3(0.0f);
	gg_Up = glm::vec3(0.0f);
	gg_Right = glm::vec3(0.0f);
	gg_WorldUp = glm::vec3(0.0f);
	gg_Rotation = glm::vec3(0.0f);

	Update();//Update Camera
}

Camera::Camera(glm::vec3 pp_StartPosition, glm::vec3 PP_StartRotation, glm::vec3 pp_WorldUp)//There
{
	gg_Front = glm::vec3(0.0f);
	gg_Up = glm::vec3(0.0f);
	gg_Right = glm::vec3(0.0f);
	gg_WorldUp = glm::vec3(0.0f);

	gg_Position = pp_StartPosition;//Set Position
	gg_WorldUp = pp_WorldUp;//Set World Up
	gg_Rotation = PP_StartRotation;//Set Rotation

	Update();//Update Camera
}

void Camera::Update()//There
{
//Calculate Front
	gg_Front.x = cos(gg_Rotation.y) * cos(gg_Rotation.x);
	gg_Front.y = sin(gg_Rotation.x);
	gg_Front.z = sin(gg_Rotation.y) * cos(gg_Rotation.x);
	gg_Front = glm::normalize(gg_Front);
//
	gg_Right = glm::normalize(glm::cross(gg_Front, gg_WorldUp));//Calculate Right
	gg_Up = glm::normalize(glm::cross(gg_Right, gg_Front));//Calculate Up
}

void Camera::SetPosition(glm::vec3 pp_NewPosition)
{
	gg_Position = pp_NewPosition;
}
void Camera::SetPosition(float pf_x, float pf_y, float pf_z)
{
	gg_Position.x = pf_x;
	gg_Position.y = pf_y;
	gg_Position.z = pf_z;
}

void Camera::Translate(glm::vec3 pv3_MoveBy)
{
	gg_Position = gg_Position + pv3_MoveBy;
}
void Camera::Translate(float pf_x, float pf_y, float pf_z)
{
	gg_Position.x += pf_x;
	gg_Position.y += pf_y;
	gg_Position.z += pf_z;
}

void Camera::SetRotation(glm::vec3 pp_NewRotation)
{
	gg_Rotation = pp_NewRotation;
}
void Camera::SetRotation(float pf_x, float pf_y, float pf_z)
{
	gg_Rotation.x = pf_x;
	gg_Rotation.y = pf_y;
	gg_Rotation.z = pf_z;
}

glm::vec3 Camera::GetPosition() const
{
	return gg_Position;
}
glm::vec3 Camera::GetRotation() const
{
	return gg_Rotation;
}

glm::vec3 Camera::GetForward() const
{
	return gg_Front;
}
glm::vec3 Camera::GetRight() const
{
	return gg_Right;
}
glm::vec3 Camera::GetUp() const
{
	return gg_Up;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(gg_Position, gg_Position + gg_Front, gg_Up);
}