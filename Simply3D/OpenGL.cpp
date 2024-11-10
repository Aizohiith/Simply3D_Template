#include "OpenGL.h"

bool OpenGL::Init(const GLint& pi_BufferWidth, const GLint& pi_BufferHeight)
{
//Initialize OpenGL API
	glewExperimental = GL_TRUE;
	GLenum ll_InitResults = glewInit();
	if (ll_InitResults != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed: " << glewGetErrorString(ll_InitResults) << std::endl;
		return false;
	}
//

	glViewport(0, 0, pi_BufferWidth, pi_BufferHeight);//Set Buffer Size
//Enable Additional Features
	glEnable(GL_DEPTH_TEST);//Enable depth rendering
	glEnable(GL_MULTISAMPLE);//Enable Anti-alising
	glEnable(GL_CULL_FACE);//Enable Face Culling
//

	return true;
}

void OpenGL::ClearScreen()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::SetWireframe(bool pb_Wireframe)
{
	if (pb_Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool OpenGL::GetWireframe()
{
	GLint polygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, polygonMode);
	return (polygonMode[0] == GL_LINE && polygonMode[1] == GL_LINE);
}
