#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <strstream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
	public:	
		Mesh();
		Mesh(const Mesh&) = delete;  // Prevent copying
		Mesh& operator=(const Mesh&) = delete;

		void Init(const std::vector<float>& Vertices, const std::vector<unsigned int>& Indices);////Init Mesh
		void LoadFromFile(std::string FileName, bool SmoothNormals);//Load Mesh
		void Render();
		void Clear();
		std::vector<float> GetVertices() const;
		std::vector<glm::vec3> GetVectorVertices() const;
		std::vector<unsigned int> GetIndices() const;
		~Mesh();
	private:
		std::vector<float> garr_Vertices;
		std::vector<unsigned int> garr_Indices;
		GLuint VAO, VBO, IBO;
		GLsizei gi_IndexCount;
};
