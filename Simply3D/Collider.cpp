#include "Collider.h"

void Collider::CalculateShape(const std::vector<float>& vert, int vertCount, glm::vec3 Rotation)
{
	// Initialize min and max vectors with extreme values
	glm::vec3 ll_Min(FLT_MAX);
	glm::vec3 ll_Max(-FLT_MAX);

	// Create a rotation matrix from the Rotation vector (assuming it's in radians)
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), (Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, (Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, (Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Iterate through each vertex in the mesh
	for (int C1 = 0; C1 < vertCount; C1 += 8)
	{
		// Extract the vertex position
		glm::vec3 vertex(vert[C1], vert[C1 + 1], vert[C1 + 2]);

		// Apply the rotation matrix to the vertex
		glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertex, 1.0f);

		// Update the min and max vectors
		ll_Min.x = std::min(ll_Min.x, rotatedVertex.x);
		ll_Min.y = std::min(ll_Min.y, rotatedVertex.y);
		ll_Min.z = std::min(ll_Min.z, rotatedVertex.z);

		ll_Max.x = std::max(ll_Max.x, rotatedVertex.x);
		ll_Max.y = std::max(ll_Max.y, rotatedVertex.y);
		ll_Max.z = std::max(ll_Max.z, rotatedVertex.z);
	}

	// Call Init with the final min and max vectors
	Init(ll_Min, ll_Max);
}

void Collider::Init(glm::vec3 pp_Min, glm::vec3 pp_Max)
{
	Min = pp_Min;
	Max = pp_Max;
}

bool ColliderAABB::IsInCollision(const Collider& otherCollider) const
{
	return otherCollider.IsInCollision(*this);
}

bool ColliderSphere::IsInCollision(const Collider& otherCollider) const
{
	return otherCollider.IsInCollision(*this);
}

glm::vec3 Collider::GetPosition() const
{
	return Position;
}
glm::vec3 Collider::GetTotalPosition() const
{
	glm::vec3 ll_Return = (Max + Min);
	ll_Return.x /= 2;
	ll_Return.y /= 2;
	ll_Return.z /= 2;
	return ll_Return + Position;
}

glm::vec3 Collider::GetScale() const
{
	return Scale;
}

glm::vec3 Collider::GetTotalScale() const
{
	glm::vec3 ll_Return = (Max - Min);
	ll_Return.x /= 2 / Scale.x;
	ll_Return.y /= 2 / Scale.y;
	ll_Return.z /= 2 / Scale.z;
	return ll_Return;
}

void Collider::SetInCollision(bool pp_InCollition)
{
	InCollision = pp_InCollition;
}
bool Collider::GetInCollision() const
{
	return InCollision;
}

void Collider::SetPosition(glm::vec3 NewPosition)
{
	Position = NewPosition;
}
void Collider::SetPosition(float x, float y, float z)
{
	Position = glm::vec3(x, y, z);
}

void Collider::SetMax(glm::vec3 NewMax)
{
	Max = NewMax;
}
void Collider::SetMax(float x, float y, float z)
{
	Max = glm::vec3(x, y, z);
}
glm::vec3 Collider::GetMax() const
{
	return Max;
}

void Collider::SetMin(glm::vec3 NewMin)
{
	Min = NewMin;
}
void Collider::SetMin(float x, float y, float z)
{
	Min = glm::vec3(x, y, z);
}
glm::vec3 Collider::GetMin() const
{
	return Min;
}

bool ColliderAABB::IsInCollision(const ColliderAABB& otherCollider) const
{
	glm::vec3 Scale = GetScale();
	glm::vec3 otherScale = otherCollider.GetScale();
	// Check for collision along all three axes (X, Y, Z)
	bool collisionX = ((Scale.x * Max.x + Position.x >= otherScale.x * otherCollider.Min.x + otherCollider.Position.x) && (Scale.x * Min.x + Position.x <= otherScale.x * otherCollider.Max.x + otherCollider.Position.x));
	bool collisionY = ((Scale.y * Max.y + Position.y >= otherScale.y * otherCollider.Min.y + otherCollider.Position.y) && (Scale.y * Min.y + Position.y <= otherScale.y * otherCollider.Max.y + otherCollider.Position.y));
	bool collisionZ = ((Scale.z * Max.z + Position.z >= otherScale.z * otherCollider.Min.z + otherCollider.Position.z) && (Scale.z * Min.z + Position.z <= otherScale.z * otherCollider.Max.z + otherCollider.Position.z));

	// If there is a collision along all axes, there is a collision
	return collisionX && collisionY && collisionZ;
}

bool ColliderAABB::IsInCollision(const ColliderSphere& otherCollider) const
{
	bool lb_InCollision = false;
	glm::vec3 Scale = GetScale();
	glm::vec3 otherScale = otherCollider.GetTotalScale();
	glm::vec3 otherPosition = otherCollider.GetPosition();
	glm::vec3 ll_Nearest_Point = glm::vec3(0.0f);

	if (otherCollider.GetPosition().x > Scale.x * Max.x + Position.x)
		ll_Nearest_Point.x = Scale.x * Max.x + Position.x;
	else
	{
		if (otherCollider.GetPosition().x < Scale.x * Min.x + Position.x)
			ll_Nearest_Point.x = Scale.x * Min.x + Position.x;
		else
			ll_Nearest_Point.x = otherCollider.GetPosition().x;
	}
		
	if (otherCollider.GetPosition().y > Scale.y * Max.y + Position.y)
		ll_Nearest_Point.y = Scale.y * Max.y + Position.y;
	else
	{
		if (otherCollider.GetPosition().y < Scale.y * Min.y + Position.y)
			ll_Nearest_Point.y = Scale.y * Min.y + Position.y;
		else
			ll_Nearest_Point.y = otherCollider.GetPosition().y;
	}

	if (otherCollider.GetPosition().z > Scale.z * Max.z + Position.z)
		ll_Nearest_Point.z = Scale.z * Max.z + Position.z;
	else
	{
		if (otherCollider.GetPosition().z < Scale.z * Min.z + Position.z)
			ll_Nearest_Point.z = Scale.z * Min.z + Position.z;
		else
			ll_Nearest_Point.z = otherCollider.GetPosition().z;
	}

	lb_InCollision = sqrtf(pow(ll_Nearest_Point.x - otherPosition.x, 2.0f) + pow(ll_Nearest_Point.y - otherPosition.y, 2.0f) + pow(ll_Nearest_Point.z - otherPosition.z, 2.0f)) <= otherScale.x;

	return lb_InCollision;
}

bool ColliderSphere::IsInCollision(const ColliderAABB& otherCollider) const
{
	bool lb_InCollision = false;
	glm::vec3 Scale = GetTotalScale();
	glm::vec3 otherScale = otherCollider.GetScale();
	glm::vec3 otherPosition = otherCollider.GetPosition();
	glm::vec3 otherMax = otherCollider.GetMax();
	glm::vec3 otherMin = otherCollider.GetMin();
	glm::vec3 ll_Nearest_Point = glm::vec3(0.0f);
	
	if (GetPosition().x > otherScale.x * otherMax.x + otherCollider.GetPosition().x)
		ll_Nearest_Point.x = otherScale.x * otherMax.x + otherCollider.GetPosition().x;
	else
	{
		if (GetPosition().x < otherScale.x * otherMin.x + otherCollider.GetPosition().x)
			ll_Nearest_Point.x = otherScale.x * otherMin.x + otherCollider.GetPosition().x;
		else
			ll_Nearest_Point.x = GetPosition().x;
	}

	if (GetPosition().y > otherScale.y * otherMax.y + otherCollider.GetPosition().y)
		ll_Nearest_Point.y = otherScale.y * otherMax.y + otherCollider.GetPosition().y;
	else
	{
		if (GetPosition().y < otherScale.y * otherMin.y + otherCollider.GetPosition().y)
			ll_Nearest_Point.y = otherScale.y * otherMin.y + otherCollider.GetPosition().y;
		else
			ll_Nearest_Point.y = GetPosition().y;
	}

	if (GetPosition().z > otherScale.z * otherMax.z + otherCollider.GetPosition().z)
		ll_Nearest_Point.z = otherScale.z * otherMax.z + otherCollider.GetPosition().z;
	else
	{
		if (GetPosition().z < otherScale.z * otherMin.z + otherCollider.GetPosition().z)
			ll_Nearest_Point.z = otherScale.z * otherMin.z + otherCollider.GetPosition().z;
		else
			ll_Nearest_Point.z = GetPosition().z;
	}

	lb_InCollision = sqrtf(pow(ll_Nearest_Point.x - Position.x, 2.0f) + pow(ll_Nearest_Point.y - Position.y, 2.0f) + pow(ll_Nearest_Point.z - Position.z, 2.0f)) <= Scale.x;

	return lb_InCollision;
}

bool ColliderSphere::IsInCollision(const ColliderSphere& otherCollider) const
{
	glm::vec3 Scale = GetTotalScale();
	glm::vec3 otherScale = otherCollider.GetTotalScale();
	glm::vec3 otherPosition = otherCollider.GetPosition();
	bool lb_InCollition = false;
	
	lb_InCollition = sqrtf(pow(Position.x - otherPosition.x, 2.0f) + pow(Position.y - otherPosition.y, 2.0f) + pow(Position.z - otherPosition.z, 2.0f)) <= Scale.x + otherScale.x;
	return lb_InCollition;
}

void ColliderAABB::SetScale(glm::vec3 NewScale)
{
	Scale = NewScale;
}

void ColliderAABB::SetScale(float x, float y, float z)
{
	Scale = glm::vec3(x, y, z);
}

void ColliderSphere::SetScale(glm::vec3 NewScale)
{
	float lf_Max = NewScale.x > NewScale.y ? NewScale.x : NewScale.y;
	lf_Max = lf_Max > NewScale.z ? lf_Max : NewScale.z;
	Scale = glm::vec3(lf_Max);
}

void ColliderSphere::SetScale(float x, float y, float z)
{
	float lf_Max = x > y ? x : y;
	lf_Max = lf_Max > z ? lf_Max : z;
	Scale = glm::vec3(lf_Max);
}