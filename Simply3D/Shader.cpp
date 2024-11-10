#include "Shader.h"

Shader::Shader()
{
	gi_ShaderID = 0;
}

void Shader::LoadFromFile(const char* ps_VertexShader, const char* ps_FragmentShader)
{
	std::string ls_VertexString = LoadFile(ps_VertexShader);//Load Vertex Shader
	std::string ls_FragmentString = LoadFile(ps_FragmentShader);//Load Fragment Shader

	Compile(ls_VertexString.c_str(), ls_FragmentString.c_str());//Compile Shader
}

void Shader::StartProgram()
{
	glUseProgram(gi_ShaderID);//Use Shader Program
}

void Shader::StopProgram()
{
	glUseProgram(0);
}

Shader::~Shader()
{
	if (gi_ShaderID != 0)
		glDeleteProgram(gi_ShaderID);
}

GLuint Shader::GetAttributeID(const char* ps_AttributeName) const
{
	return glGetUniformLocation(gi_ShaderID, ps_AttributeName);
}

GLuint Shader::GetShaderID() const
{
	return gi_ShaderID;
}

std::string Shader::LoadFile(const char* ps_FileLocation)
{
	std::string ls_Result;
	std::ifstream pp_FileStream(ps_FileLocation, std::ios::in);

	if (!pp_FileStream.is_open()) {
		std::cerr << "LoadFile FAILED: " << ps_FileLocation << " Does Not Exist" << std::endl;
		return "";
	}

	std::string ls_Line = "";
	while (!pp_FileStream.eof())
	{
		std::getline(pp_FileStream, ls_Line);
		ls_Result.append(ls_Line + "\n");
	}

	pp_FileStream.close();
	return ls_Result;
}

void Shader::Compile(const char* ps_VertexShader, const char* ps_FragmentShader)
{
//Create Shader Program
	gi_ShaderID = glCreateProgram();

	if (!gi_ShaderID)
	{
		std::cerr << "Failed to create shader" << std::endl;
		return;
	}
//
//Add Shader
	Add(gi_ShaderID, ps_VertexShader, GL_VERTEX_SHADER);
	Add(gi_ShaderID, ps_FragmentShader, GL_FRAGMENT_SHADER);
//
//Link Shader Program
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(gi_ShaderID);
	glGetProgramiv(gi_ShaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(gi_ShaderID, sizeof(eLog), NULL, eLog);
		std::cerr << "Error linking program: " << eLog << std::endl;
		return;
	}
//
//Validate Shader Program
	glValidateProgram(gi_ShaderID);
	glGetProgramiv(gi_ShaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(gi_ShaderID, sizeof(eLog), NULL, eLog);
		std::cerr << "Error validating program: " << eLog << std::endl;
		return;
	}
//
}

void Shader::Add(GLuint pi_ProgramID, const char* ps_ShaderCode, GLenum pp_ShaderType)
{
	GLuint gi_ShaderID = glCreateShader(pp_ShaderType);//Create Shader
//Set Shader Code
	const GLchar* ls_ShaderCode[1] = { ps_ShaderCode };
	GLint li_CodeLength[1] = { static_cast<GLint>(strlen(ps_ShaderCode)) };
	glShaderSource(gi_ShaderID, 1, ls_ShaderCode, li_CodeLength);
//
//Compile Shader
	glCompileShader(gi_ShaderID);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(gi_ShaderID, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(gi_ShaderID, 1024, NULL, eLog);
		std::cerr << "Error compiling the shader of type: " << pp_ShaderType <<  " With message: " << eLog << std::endl;
		return;
	}
//

	glAttachShader(pi_ProgramID, gi_ShaderID);//Attach Shader
}

void Shader::SetAttribute(GLuint pi_ID, float pf_Value) const
{
	glUniform1f(pi_ID, pf_Value);
}
void Shader::SetAttribute(GLuint pi_ID,const glm::vec2& pv_Value) const
{
	glUniform2f(pi_ID, pv_Value.x, pv_Value.y);
}
void Shader::SetAttribute(GLuint pi_ID,const glm::vec3& pv_Value) const
{
	glUniform3f(pi_ID, pv_Value.x, pv_Value.y, pv_Value.z);
}
void Shader::SetAttribute(GLuint pi_ID, const glm::vec4& pv_Value) const
{
	glUniform4f(pi_ID, pv_Value.x, pv_Value.y, pv_Value.z, pv_Value.w);
}
void Shader::SetAttribute(GLuint pi_ID, const glm::mat4& pm_Value) const
{
	glUniformMatrix4fv(pi_ID, 1, GL_FALSE, glm::value_ptr(pm_Value));
}

void Shader::SetAttribute(GLuint pi_ID, const GLuint pi_TextreUnit) const
{
	glUniform1i(pi_ID, pi_TextreUnit);
}