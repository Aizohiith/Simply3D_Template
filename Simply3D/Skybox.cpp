#include "Skybox.h"
#include "CommonValues.hpp"

Skybox::Skybox()
{
	SkyMesh = nullptr;
	SkyShader = nullptr;

	TextureID = 0;
	ViewID = 0;
	ProjectionID = 0;
}

Skybox::~Skybox()
{
	delete SkyMesh;
	delete SkyShader;
	glDeleteTextures(1, &TextureID);
	ViewID = 0;
	ProjectionID = 0;
	TextureID = 0;
}

Skybox::Skybox(std::vector<std::string> Images, bool pb_HasAlpha)
{

	SkyShader = new Shader();//Create Skybox Shader
	SkyShader->LoadFromFile("Simply3D/Shaders/Sky.vert", "Simply3D/Shaders/Sky.frag");//Load Shader
//Get Shader IDs
	ProjectionID = SkyShader->GetAttributeID("projection");
	ViewID = SkyShader->GetAttributeID("view");
//

	glGenTextures(1, &TextureID);//Generate Texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);//Bind Texture
//Set Paramaters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
	int w, h, b;
	int Counter = 0;
	for (auto& C1 : Images)
	{
		stbi_set_flip_vertically_on_load(true);//Flip Texture
	//Load Texture
		unsigned char* TextureData = stbi_load(C1.c_str(), &w, &h, &b, 0);

		if (TextureData == nullptr)
		{
			std::cout << "Load Error: " << C1 << std::endl;
			return;
		}
	//
	//Apply Texture
		if (pb_HasAlpha)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Counter, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
		else
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Counter, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);

		Counter++;
	//

		stbi_image_free(TextureData);//Free Image
	}

//Create Skybox Indecies
	std::vector<unsigned int> skyboxIndices = 
	{
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};
//
//Create Skybox Vertices
	std::vector<float> skyboxVertices = 
	{
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};
//
	SkyMesh = new Mesh();//Create Skybox Mesh
	SkyMesh->Init(skyboxVertices, skyboxIndices);//Init Mesh
}

void Skybox::Render(glm::mat4 viewMat, glm::mat4 Project)
{

	viewMat = glm::mat4(glm::mat3(viewMat));

	glDepthMask(GL_FALSE);//Disable Depth Mask

	SkyShader->StartProgram();//Start Skybox Shader

	SkyShader->SetAttribute(ProjectionID, Project);//Set Projection Matrix
	SkyShader->SetAttribute(ViewID, viewMat);//Set View Matrix

	glActiveTexture(GL_TEXTURE0);//Activate Texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);//Bind Texture

	SkyMesh->Render();//Render Model
	SkyShader->StopProgram();//Stop Skybox Shader
	glDepthMask(GL_TRUE);//Enable Depth Mask
}