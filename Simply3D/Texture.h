#pragma once
#include <GL/glew.h>
#include <iostream>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	void Load(std::string FileName, bool HasAlpha);
	void Use(GLenum TextureUnit);
	void Clear();

private:
	int Width, Height;
	int BitDepth;
	GLuint TextureID;
};

