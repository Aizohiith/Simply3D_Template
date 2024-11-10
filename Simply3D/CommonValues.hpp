#ifndef COMMONVALES
#define COMMONVALES
#include "stb_image.h"
#define LOG_DEBUG3D(condition, message) if (condition) std::cout << "Log: " << message << std::endl;

//static const float MAX_VIEWDIST = 350.0f;
static const float MAX_VIEWDIST = 10000.0f;
static const float MIN_VIEWDIST = 0.1f;
static const float FOV = 45.0f;
static const float RENDER_FPS = 15.0f;

#endif // !COMMONVALES


