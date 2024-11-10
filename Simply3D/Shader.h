#pragma once

#include <GL\glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>

class Shader
{
	public:
		Shader();
		void LoadFromFile(const char* VertexShader, const char* FragmentShader);// Load shaders
		void StartProgram();
		static void StopProgram();

		void SetAttribute(GLuint ID, float Value) const;
		void SetAttribute(GLuint ID, const glm::vec2& Value) const;
		void SetAttribute(GLuint ID, const glm::vec3& Value) const;
		void SetAttribute(GLuint ID, const glm::vec4& Value) const;
		void SetAttribute(GLuint ID, const glm::mat4& Value) const;
		void SetAttribute(GLuint ID, const GLuint TextreUnit) const;

		~Shader();

		GLuint GetAttributeID(const char* AttributeName) const;
		GLuint GetShaderID() const;

	private:
		GLuint gi_ShaderID;

		std::string LoadFile(const char* FileName);
		void Compile(const char* VertexShader, const char* FragmentShader);//Compile Shader
		void Add(GLuint ProgramID, const char* ShaderCode, GLenum ShaderType);//Add Shader
};