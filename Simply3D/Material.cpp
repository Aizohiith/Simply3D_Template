#include "Material.h"

Material::Material()
{
	SpecularIntencity = 1.0f;
	Shininesss = 1.0f;
	Smooth = false;
}

Material::Material(float SInt, float Sness, bool pb_Smooth)
{
	SpecularIntencity = SInt;
	Shininesss = Sness;
	Smooth = pb_Smooth;
}

void Material::Use(GLuint IntencityID, GLuint ShinessID)
{
	glUniform1f(IntencityID, SpecularIntencity);
	glUniform1f(ShinessID, Shininesss);
}

void Material::SetSmooth(bool NewSmooth)
{
	Smooth = NewSmooth;
}

bool Material::GetSmooth() const
{
	return Smooth;
}

void Material::SetIntencity(float NewIntencity)
{
	SpecularIntencity = NewIntencity;

	if (SpecularIntencity > 10.0f)
		SpecularIntencity = 10.0f;

	if (SpecularIntencity < 0.0f)
		SpecularIntencity = 0.0f;
}
float Material::GetIntencity() const
{
	return SpecularIntencity;
}

void Material::SetShininesss(float NewShininesss)
{
	if (SpecularIntencity > 256.0f)
		SpecularIntencity = 256.0f;

	if (SpecularIntencity < 1.0f)
		SpecularIntencity = 1.0f;
	Shininesss = NewShininesss;
}
float Material::GetShininesss() const
{
	return Shininesss;
}