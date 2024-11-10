#pragma once

#include <GL/glew.h>


class Material
{
public:
	Material();
	Material(float SInt,float Sness, bool Smooth);
	
	void Use(GLuint IntencityID, GLuint ShinessID);

	void SetIntencity(float NewIntencity);
	float GetIntencity() const;

	void SetShininesss(float NewShininesss);
	float GetShininesss() const;

	void SetSmooth(bool NewSmooth);
	bool GetSmooth() const;

	private:
		float SpecularIntencity;
		float Shininesss;
		bool Smooth;
};

