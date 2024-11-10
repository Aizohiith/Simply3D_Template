#pragma once

#include <gl/glew.h>
#include <stdio.h>
class Shadow
{

public:

	Shadow();
	~Shadow();
	bool Init(GLuint Width, GLuint Height);
	void Write();
	void Read(GLenum TextureUnit);

	GLuint GetShadowMap() const;

	unsigned int GetWidth() const;
	unsigned GetHeight() const;

protected:
	GLuint FBO, ShadowMap;
	GLuint Width, Height;
};

