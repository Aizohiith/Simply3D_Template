#include "Texture.h"

#include "CommonValues.hpp"

Texture::Texture()
{
	Width = 0;
	Height = 0;
	TextureID = 0;
	BitDepth = 0;

}

Texture::~Texture()
{
	Clear();
}


void Texture::Load(std::string ps_FileName, bool pb_HasAlpha)
{
	Clear();
	stbi_set_flip_vertically_on_load(true);//Flip Texture
//Load From File
	unsigned char* TextureData = nullptr;

	if (pb_HasAlpha)
		TextureData = stbi_load(ps_FileName.c_str(), &Width, &Height, &BitDepth, STBI_rgb_alpha);
	else
		TextureData = stbi_load(ps_FileName.c_str(), &Width, &Height, &BitDepth, 0);

	if (TextureData == nullptr)
	{
		std::cout << "Can't load Texture: " << ps_FileName  << std::endl;
		return;
	}
//
	glGenTextures(1, &TextureID);//Generate Texture
	glBindTexture(GL_TEXTURE_2D, TextureID);//Bind Texture
//Set Paramaters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (pb_HasAlpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
//
	glGenerateMipmap(GL_TEXTURE_2D);//Generate  Mipmap

	glBindTexture(GL_TEXTURE_2D, 0);//Unbind Texture

	stbi_image_free(TextureData);//Free Texture Data

}

void Texture::Clear()
{
	glDeleteTextures(1, &TextureID);
	Width = 0;
	Height = 0;
	TextureID = 0;
	BitDepth = 0;
}

void Texture::Use(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);//Activate Texure
	glBindTexture(GL_TEXTURE_2D, TextureID);//Bind Texure
}
