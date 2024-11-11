#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include <algorithm> // For std::min and std::max
#include <cfloat> // For FLT_MAX

class ColliderAABB;
class ColliderSphere;

class Collider
{
	public:
		
		void Init(glm::vec3 Min, glm::vec3 Max);

		void SetPosition(glm::vec3 NewPosition);
		void SetPosition(float x, float y, float z);
		glm::vec3 GetPosition() const;
		glm::vec3 GetTotalPosition() const;

		void SetInCollision(bool InCollition);
		bool GetInCollision() const;

		virtual bool IsInCollision(const Collider& otherCollider) const = 0;
		virtual bool IsInCollision(const ColliderAABB& otherCollider) const = 0;
		virtual bool IsInCollision(const ColliderSphere& otherCollider) const = 0;

		void SetMax(glm::vec3 NewMax);
		void SetMax(float x, float y, float z);
		glm::vec3 GetMax() const;

		void SetMin(glm::vec3 NewMin);
		void SetMin(float x, float y, float z);
		glm::vec3 GetMin() const;

		virtual void SetScale(glm::vec3 NewScale) = 0;
		virtual void SetScale(float x, float y, float z) = 0;
		glm::vec3 GetScale() const;
		glm::vec3 GetTotalScale() const;

		void CalculateShape(const std::vector<float>& vert, int vertCount, glm::vec3 Rotation);

	protected:
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 Min;
		glm::vec3 Max;
		bool InCollision;
};

class  ColliderAABB : public  Collider
{
	void SetScale(glm::vec3 NewScale) override;
	void SetScale(float x, float y, float z) override;
	bool IsInCollision(const ColliderAABB& otherCollider) const override;
	bool IsInCollision(const ColliderSphere& otherCollider) const override;
	bool IsInCollision(const Collider& otherCollider) const override;
};

class  ColliderSphere : public  Collider
{
	void SetScale(glm::vec3 NewScale) override;
	void SetScale(float x, float y, float z) override;
	bool IsInCollision(const ColliderAABB& otherCollider) const override;
	bool IsInCollision(const ColliderSphere& otherCollider) const override;
	bool IsInCollision(const Collider& otherCollider) const override;
};