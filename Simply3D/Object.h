#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Enumerators.hpp"	
#include "Material.h"
#include "Collider.h"

class Object
{
public:
	Object();
//Init Object
	void Init(std::string MeshFileName, std::string TextureFileName, ShaderTypes ShaderType, const Material MaterialType, bool TextureHasAlpha = true);
	void Init(std::string ps_MeshFileName, ShaderTypes pp_ShaderType, const Material MaterialType, bool TextureHasAlpha = true);
	void Init(ObjectTypes Object, std::string TextureFileName, ShaderTypes ShaderType, const Material MaterialType, bool TextureHasAlpha = true);
	void Init(ObjectTypes Object, ShaderTypes ShaderType, const Material MaterialType, bool TextureHasAlpha = true);

//
	void Render();
	void Update();
	glm::mat4 GetTransformation() const;

	void SetColliderType(ColliderTypes NewColliderType);
	Collider* GetCollider() const;
	ColliderTypes GetColliderType() const;

	void SetColor(glm::vec3 NewColor);
	void SetColor(float R, float G, float B);
	glm::vec3 GetColor() const;

	void SetScale(float x, float y, float z);
	void SetScale(glm::vec3 newScale);
	glm::vec3 GetScale() const;

	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 NewPosition);
	glm::vec3 GetPosition() const;

	void SetRotation(float x, float y, float z);
	void SetRotation(glm::vec3 NewRotation);
	glm::vec3 GetRotation() const;

	void Translate(float x, float y, float z);
	void Translate(glm::vec3 TranslateBy);

	void Rotate(float x, float y, float z);
	void Rotate(glm::vec3 RotateBy);


	void SetVisible(bool Visible);
	bool GetVisible() const;
	
	void SetShaderType(ShaderTypes NewShaderType);
	ShaderTypes GetShaderType() const;

	void SetMaterial(const Material NewMaterial);
	Material GetMaterial() const;

	void SetMesh(Mesh* newMesh);
	Mesh* GetMesh() const;

	void SetTexture(Texture* newTexture);
	Texture* GetTexture() const;

	void SetUpdateCollider(bool UpdateCollider);
	bool GetUpdateCollider() const;
	
	void SetCastShadow(bool CastShadow);
	bool GetCastShadow() const;
	void SetRecieveShadow(bool RecieveShadow);
	bool GetRecieveShadow() const;


	void Clear();
	~Object();
private:
	Material gg_Material;
	Texture* gg_Texture;
	Collider* gg_Collider;
	Mesh* gg_Mesh;
	ShaderTypes gg_ShaderType;
	ColliderTypes gg_ColliderType;
	bool gb_Visible;
	bool gb_UpdateCollider;
	bool gb_CastShadow;
	bool gb_RecieveShadow;

	glm::vec3 Color;
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::mat4 Transformation;

	void ResetTransformation();
	void LoadPresetOJBs(ObjectTypes Object);//Load Preset Object
};
