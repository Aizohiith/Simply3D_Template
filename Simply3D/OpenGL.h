#pragma once

#include <iostream>
#include <GL/glew.h>

namespace OpenGL
{
	bool Init(const GLint& BufferWidth,const GLint& BufferHeight);
	void ClearScreen();

	void SetWireframe(bool);
	bool GetWireframe();
};

