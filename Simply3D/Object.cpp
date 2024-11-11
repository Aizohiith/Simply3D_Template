#include "Object.h"
#include "CommonValues.hpp"

Object::Object()
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	Color = glm::vec3(1.0f);

	gg_Collider = nullptr;
	gb_Visible = true;
	gb_UpdateCollider = true;
	gb_CastShadow = true;
	gb_RecieveShadow = true;
	gg_Texture = nullptr;
	gg_Mesh = nullptr;

	gg_ColliderType = NoColliderType;
	Update();
	ResetTransformation();
}

void Object::Init(std::string ps_MeshFileName, std::string ps_TextureFileName, ShaderTypes pp_ShaderType,const Material pp_MaterialType, bool HasAlpha)
{
	Clear();
	gg_ShaderType = pp_ShaderType;//Set Object Shader
	gg_Material = pp_MaterialType;//Set Object Material
	gg_Texture = new Texture();//Create Object Texture
	gg_Texture->Load(ps_TextureFileName, HasAlpha);//Load Object Texture
	gg_Mesh = new Mesh();//Create Mesh
	gg_Mesh->LoadFromFile(ps_MeshFileName, gg_Material.GetSmooth());//Load Mesh
}

void Object::Init(std::string ps_MeshFileName, ShaderTypes pp_ShaderType, const Material pp_MaterialType, bool HasAlpha)
{
	Clear();
	gg_ShaderType = pp_ShaderType;//Set Object Shader
	gg_Material = pp_MaterialType;//Set Object Material
	gg_Texture = new Texture();//Create Object Texture
	gg_Texture->Load("Simply3D/Textures/Plane.png", HasAlpha);//Load Object Texture
	gg_Mesh = new Mesh();//Create Mesh
	gg_Mesh->LoadFromFile(ps_MeshFileName, gg_Material.GetSmooth());//Load Mesh
}
void Object::Init(ObjectTypes pp_Object, ShaderTypes pp_ShaderType, const Material pp_MaterialType, bool HasAlpha)
{
	Clear();
	gg_ShaderType = pp_ShaderType;//Set Object Shader
	gg_Material = pp_MaterialType;//Set Object Material
	gg_Texture = new Texture();//Create Object Texture
	gg_Texture->Load("Simply3D/Textures/Plane.png", HasAlpha);//Load Object Texture
	LoadPresetOJBs(pp_Object);//Load Preset Object
}

void Object::Init(ObjectTypes pp_Object, std::string ps_TextureFileName, ShaderTypes pp_ShaderType, const Material pp_MaterialType, bool HasAlpha)
{
	Clear();
	gg_ShaderType = pp_ShaderType;//Set Object Shader
	gg_Material = pp_MaterialType;//Set Object Material
	gg_Texture = new Texture();//Create Object Texture
	gg_Texture->Load(ps_TextureFileName, HasAlpha);//Load Object Texture
	LoadPresetOJBs(pp_Object);//Load Preset Object

}

void Object::LoadPresetOJBs(ObjectTypes pp_Object)
{
	gg_Mesh = new Mesh();//Create Mesh
//Load Mesh
	switch (pp_Object)
	{
		case CubeObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/Cube.obj", gg_Material.GetSmooth()); break;
		case PlaneObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/plane.obj", gg_Material.GetSmooth()); break;
		case WallObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/Wall.obj", gg_Material.GetSmooth()); break;
		case SphereObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/Sphere.obj", gg_Material.GetSmooth()); break;
		case DonutObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/Donut.obj", gg_Material.GetSmooth()); break;
		case ConeObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/Cone.obj", gg_Material.GetSmooth()); break;
		case CylinderObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/Cylinder.obj", gg_Material.GetSmooth()); break;
		case IcosphereObjectType: gg_Mesh->LoadFromFile("Simply3D/Objects/Icosphere.obj", gg_Material.GetSmooth()); break;

		default: std::cerr << "Invalid Object to Add" << std::endl;  break;
	}
//
}

void Object::Render()
{
	if (!gb_Visible)
		return;
	if (!gg_Texture)
		return;
	if (!gg_Mesh)
		return;
	gg_Texture->Use(GL_TEXTURE0);
	gg_Mesh->Render();//Render Model
}

void Object::Update()
{
	if (gg_ColliderType == NoColliderType)
		return;
	if (not gb_UpdateCollider)
		return;
	gg_Collider->SetPosition(Position);
	gg_Collider->SetScale(Scale);
}

void Object::SetScale(float pf_x, float pf_y, float pf_z)
{
	Scale.x = pf_x;
	Scale.y = pf_y;
	Scale.z = pf_z;
	ResetTransformation();
}
void Object::SetScale(glm::vec3 newScale)
{
	Scale = newScale;
	ResetTransformation();
}

glm::vec3 Object::GetScale() const
{
	return Scale;
}

void Object::SetPosition(float pf_x, float pf_y, float pf_z)
{
	Position.x = pf_x;
	Position.y = pf_y;
	Position.z = pf_z;
	ResetTransformation();
}

void Object::SetPosition(glm::vec3 pv_NewPos)
{
	Position = pv_NewPos;
	ResetTransformation();
}

void Object::SetRotation(float pf_x, float pf_y, float pf_z)
{
	Rotation.x = pf_x;
	Rotation.y = pf_y;
	Rotation.z = pf_z;
	ResetTransformation();
}

void Object::SetRotation(glm::vec3 pv_NewRotation)
{
	Rotation = pv_NewRotation;
	ResetTransformation();
}

void Object::Translate(float pf_x, float pf_y, float pf_z)
{
	Position.x += pf_x;
	Position.y += pf_y;
	Position.z += pf_z;
	ResetTransformation();
}

void Object::Translate(glm::vec3 pp_TranslateBy)
{
	Position += pp_TranslateBy;
	ResetTransformation();
}


void Object::Rotate(float pf_x, float pf_y, float pf_z)
{

	glm::mat4 ll_Rotation(1.0f);
	ll_Rotation = glm::rotate(ll_Rotation, pf_x, glm::vec3(1, 0, 0));
	ll_Rotation = glm::rotate(ll_Rotation, pf_y, glm::vec3(0, 1, 0));
	ll_Rotation = glm::rotate(ll_Rotation, pf_z, glm::vec3(0, 0, 1));
	Position = glm::vec3(ll_Rotation * glm::vec4(Position, 1.0f));

	ResetTransformation();
}


Collider* Object::GetCollider() const
{
	return gg_Collider;
}
void Object::SetColliderType(ColliderTypes NewColliderType)
{
	if (gg_Collider != nullptr)
	{
		delete gg_Collider;
		gg_Collider = nullptr;
	}

	if (NewColliderType == ColliderTypes::NoColliderType)
	{
		gg_ColliderType = ColliderTypes::NoColliderType;
		return;
	}

	if (NewColliderType == BoxColliderType)
	{
		gg_Collider = new ColliderAABB();
		gg_Collider->Init(glm::vec3(-1.0f), glm::vec3(1.0f));
		gg_ColliderType = BoxColliderType;
		gg_Collider->CalculateShape(gg_Mesh->GetVertices(), gg_Mesh->GetVertices().size(), Rotation);
		Update();
		return;
	}

	if (NewColliderType == SphereColliderType)
	{
		gg_Collider = new ColliderSphere();
		gg_Collider->Init(glm::vec3(-1.0f), glm::vec3(1.0f));
		gg_ColliderType = SphereColliderType;
		gg_Collider->CalculateShape(gg_Mesh->GetVertices(), gg_Mesh->GetVertices().size(), Rotation);
		Update();
		return;
	}
}

void Object::SetColor(glm::vec3 NewColor)
{
	Color.x = NewColor.x;
	Color.y = NewColor.y;
	Color.z = NewColor.z;
}

void Object::SetColor(float pi_R, float pi_G, float pi_B)
{
	Color.x = pi_R;
	Color.y = pi_G;
	Color.z = pi_B;
}


void Object::Rotate(glm::vec3 pv_RotateBy)
{
	glm::mat4 ll_Rotation(1.0f);
	ll_Rotation = glm::rotate(ll_Rotation, pv_RotateBy.x, glm::vec3(1, 0, 0));
	ll_Rotation = glm::rotate(ll_Rotation, pv_RotateBy.y, glm::vec3(0, 1, 0));
	ll_Rotation = glm::rotate(ll_Rotation, pv_RotateBy.z, glm::vec3(0, 0, 1));
	Position = glm::vec3(ll_Rotation * glm::vec4(Position, 1.0f));

	ResetTransformation();
}


glm::vec3 Object::GetPosition() const
{
	return Position;
}
void Object::SetShaderType(ShaderTypes NewShaderType)
{
	gg_ShaderType = NewShaderType;
}
ShaderTypes Object::GetShaderType() const
{
	return gg_ShaderType;
}
ColliderTypes Object::GetColliderType() const
{
	return gg_ColliderType;
}
glm::mat4 Object::GetTransformation() const
{
	return Transformation;
}

void Object::SetVisible(bool Visible)
{
	gb_Visible = Visible;
}
bool Object::GetVisible() const
{
	return gb_Visible;
}

glm::vec3 Object::GetColor() const
{
	return Color;
}

void Object::ResetTransformation()
{
	Transformation = glm::mat4(1.0f);

	Transformation = glm::translate(Transformation, Position);
	Transformation = glm::rotate(Transformation, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	Transformation = glm::rotate(Transformation, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	Transformation = glm::rotate(Transformation, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	Transformation = glm::scale(Transformation, Scale);
}

void Object::SetMaterial(Material pp_NewMaterial)
{
	gg_Material = pp_NewMaterial;
}
Material Object::GetMaterial() const
{
	return gg_Material;
}

glm::vec3 Object::GetRotation() const
{
	return Rotation;
}

void Object::SetMesh(Mesh* newMesh)
{
	gg_Mesh = newMesh;
}

Mesh* Object::GetMesh() const
{
	return gg_Mesh;
}

void Object::SetTexture(Texture* newTexture)
{
	gg_Texture = newTexture;
}
Texture* Object::GetTexture() const
{
	return gg_Texture;
}

void Object::SetUpdateCollider(bool UpdateCollider)
{
	gb_UpdateCollider = UpdateCollider;
}
bool Object::GetUpdateCollider() const
{
	return gb_UpdateCollider;
}

void Object::SetCastShadow(bool CastShadow)
{
	gb_CastShadow = CastShadow;
}
bool Object::GetCastShadow() const
{
	return gb_CastShadow;
}

void Object::SetRecieveShadow(bool RecieveShadow)
{
	gb_RecieveShadow = RecieveShadow;
}
bool Object::GetRecieveShadow() const
{
	return gb_RecieveShadow;
}

void Object::Clear()
{
	// Delete dynamically allocated resources if they exist
	if (gg_Texture) {
		delete gg_Texture;
		gg_Texture = nullptr;  // Set to nullptr to avoid dangling pointer
	}

	if (gg_Collider) {
		delete gg_Collider;
		gg_Collider = nullptr;  // Set to nullptr to avoid dangling pointer
	}

	if (gg_Mesh) {
		delete gg_Mesh;
		gg_Mesh = nullptr;  // Set to nullptr to avoid dangling pointer
	}
}

Object::~Object()
{
	Clear();
}