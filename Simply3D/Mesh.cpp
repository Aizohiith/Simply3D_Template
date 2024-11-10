#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	gi_IndexCount = 0;
}


void Mesh::Init(const std::vector<float>& parr_Vertices, const std::vector<unsigned int>& parr_Indices)
{
	Clear();
	garr_Vertices = parr_Vertices;
	garr_Indices = parr_Indices;
	gi_IndexCount = static_cast<int>(parr_Indices.size());

	glGenVertexArrays(1, &VAO);//Generate Vertex Array
	glBindVertexArray(VAO);//Bind Vertex Array


	glGenBuffers(1, &IBO);//Generate Buffer For Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);//Bind Buffer For Indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(parr_Indices[0]) * gi_IndexCount, &parr_Indices[0], GL_STATIC_DRAW);//Set Buffer Data For Indices

	glGenBuffers(1, &VBO);//Generate Buffer For Vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Bind Buffer For Vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(parr_Vertices[0]) * static_cast<int>(parr_Vertices.size()), &parr_Vertices[0], GL_STATIC_DRAW);//Set Buffer Data For Vertices

//Set Vertext Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(parr_Vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
//
//Set Texture Pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(parr_Vertices[0]) * 8, (void*)(sizeof(parr_Vertices[0]) * 3));//Texture
	glEnableVertexAttribArray(1);
//
//Set Normal Pointer
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(parr_Vertices[0]) * 8, (void*)(sizeof(parr_Vertices[0]) * 5));//Normal Data
	glEnableVertexAttribArray(2);
//

	glBindBuffer(GL_ARRAY_BUFFER, 0);//Unbind Buffer For Vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//Unbind Buffer For Indices

	glBindVertexArray(0);//Unbind Vertex Array
}


void Mesh::LoadFromFile(std::string ps_FileName, bool pb_SmoothNormals)
{
	std::vector<float> larr_Verteces;
	std::vector<unsigned int> larr_Indeces;
//Create Importer
	Assimp::Importer importer;

	const aiScene* scene = nullptr;
//
//Load Object File
	if (pb_SmoothNormals)
	{
		scene = importer.ReadFile(ps_FileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	}
	else
	{
		scene = importer.ReadFile(ps_FileName, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
	}

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
		return;
	}
//
//Extracet Data
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		const aiMesh* mesh = scene->mMeshes[i];

		// Process vertices
		for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
			const aiVector3D& pos = mesh->mVertices[j];
			const aiVector3D& uv = mesh->mTextureCoords[0][j];
			const aiVector3D& normal = mesh->mNormals[j];

			larr_Verteces.push_back(pos.x);
			larr_Verteces.push_back(pos.y);
			larr_Verteces.push_back(pos.z);

			if (mesh->HasTextureCoords(0))
			{
				larr_Verteces.push_back(uv.x);
				larr_Verteces.push_back(uv.y);
			}
			else
			{
				larr_Verteces.push_back(0);
				larr_Verteces.push_back(0);
			}

			larr_Verteces.push_back(normal.x);
			larr_Verteces.push_back(normal.y);
			larr_Verteces.push_back(normal.z);
		}

		// Process indices
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
			const aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; ++k) {
				larr_Indeces.push_back(face.mIndices[k]);
			}
		}


		//std::cout << "Mesh " << i << " Vertices: " << larr_Verteces.size() / 8 << ", Indices: " << larr_Indeces.size() << std::endl;
	}
//
	importer.FreeScene();//Free Importer
	Init(larr_Verteces, larr_Indeces);//Init Mesh
}
std::vector<glm::vec3> Mesh::GetVectorVertices() const
{
	std::vector<glm::vec3> vectorVertices;

	// Ensure vectorVertices has enough space (optional but efficient)
	vectorVertices.reserve(garr_Indices.size());

	for (unsigned int index : garr_Indices)
	{
		// Assuming each vertex is stored as three consecutive floats (x, y, z)
		glm::vec3 vertex(
			garr_Vertices[index * 3],     // x component
			garr_Vertices[index * 3 + 1], // y component
			garr_Vertices[index * 3 + 2]  // z component
		);
		vectorVertices.push_back(vertex);
	}

	return vectorVertices;
}
std::vector<float> Mesh::GetVertices() const
{
	return garr_Vertices;
}
std::vector<unsigned int> Mesh::GetIndices() const
{
	return garr_Indices;
}

void Mesh::Render()
{
	glBindVertexArray(VAO);//Bind Vertex Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);//Bind Indext Buffer
	glDrawElements(GL_TRIANGLES, gi_IndexCount, GL_UNSIGNED_INT, 0);//Draw Triangels
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//Unbind Index Buffer
	glBindVertexArray(0);//Unbind Vertex Array
}

void Mesh::Clear()
{
	// Clean up VBO
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;  // Set to 0 to avoid dangling reference
	}

	// Clean up IBO
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;  // Set to 0 to avoid dangling reference
	}

	// Clean up VAO
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;  // Set to 0 to avoid dangling reference
	}

	gi_IndexCount = 0;

	garr_Indices.clear();
	garr_Vertices.clear();
}

Mesh::~Mesh()
{
	Clear();
}
