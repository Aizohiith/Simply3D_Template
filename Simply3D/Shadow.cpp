#include "Shadow.h"

Shadow::Shadow()
{
	FBO = 0;
	ShadowMap = 0;
}

bool Shadow::Init(GLuint pi_Width, GLuint pi_Height)
{
//Set Size
	Width = pi_Width;
	Height = pi_Height;
//

	glGenFramebuffers(1, &FBO);//Generate Framebuffer

	glGenTextures(1, &ShadowMap);//Generate Texture
	glBindTexture(GL_TEXTURE_2D, ShadowMap);//Bind Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);//Set Texture Base Properties
//Set Texture Prossesing Properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//Set Texture Border Color
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);//Bind Frame Buffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap, 0);//Bind Frame Buffer To Texture

	glDrawBuffer(GL_NONE);//Disable Draw Buffer
	glReadBuffer(GL_NONE);//Disable Read Buffer

//Check Framebuffer Status
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %s\n", Status);
		return false;
	}
//
	return true;
}

void Shadow::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void Shadow::Read(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, ShadowMap);
}

GLuint Shadow::GetShadowMap() const
{
	return ShadowMap;
}

unsigned int Shadow::GetWidth() const
{
	return Width;
}
unsigned Shadow::GetHeight() const
{
	return Height;
}

Shadow::~Shadow()
{
	if (FBO)
		glDeleteFramebuffers(1, &FBO);
	if (ShadowMap)
		glDeleteTextures(1, &ShadowMap);
}