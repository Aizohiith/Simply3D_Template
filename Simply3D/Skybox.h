#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> ImageList, bool HasAlpha = false);
	void Render(glm::mat4 viewMat, glm::mat4 Project);
	~Skybox();

private:
	Mesh* SkyMesh;
	Shader* SkyShader;

	GLuint TextureID;
	GLuint ViewID;
	GLuint ProjectionID;


};

