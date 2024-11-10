#include "Window.h"



bool Window::gb_Focused = true;

bool Window::Init(const char* ps_Name, int pi_Width, int pi_Height)
{
//Initialize parameters
	gb_Focused = true;
	gi_Width = pi_Width;
	gi_Height = pi_Height;
	gb_KeyHandlerTriggerd = false;
	gb_MouseHandlerTriggerd = false;
	gb_MouseButtonHandlerTriggerd = false;
	gb_WindowResizeHandlerTriggerd = true;
//
//Initialize Window API
	if (!glfwInit())
	{
		std::cerr << "glfwInit FAILED" << std::endl;
		glfwTerminate();
		return false;
	}
//
//Set Vertion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
//
//Create Window
	gg_MainWindow = glfwCreateWindow(gi_Width, gi_Height, ps_Name, NULL, NULL);

	if (gg_MainWindow == nullptr)
	{
		std::cerr << "glfwCreateWindow FAILED" << std::endl;
		glfwTerminate();
		return false;
	}
	
//
//Setup Buffer
	glfwGetFramebufferSize(gg_MainWindow, &gi_BufferWidth, &gi_BufferHeight);
	glfwMakeContextCurrent(gg_MainWindow);
	glfwSetWindowUserPointer(gg_MainWindow, this);
//
//Setup Callbacks
	glfwSetKeyCallback(gg_MainWindow, KeyboardHandeler);
	glfwSetCursorPosCallback(gg_MainWindow, MouseHandler);
	glfwSetMouseButtonCallback(gg_MainWindow, MouseButtonHandler);
	glfwSetWindowSizeCallback(gg_MainWindow, WindowResizeHandler);
	glfwSetWindowFocusCallback(gg_MainWindow, FocusHandler);
//
//Center Window
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
	glfwSetWindowMonitor(gg_MainWindow, nullptr, mode->width / 2 - gi_Width / 2, mode->height / 2 - gi_Height / 2, gi_Width, gi_Height, 0);
//
	return true;
}

void Window::SetLockMouse(const bool& pb_Locked)
{
	if (pb_Locked)
		glfwSetInputMode(gg_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(gg_MainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Window::GetLockMouse() const
{
	return glfwGetInputMode(gg_MainWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

void Window::EventHandler()
{
	glfwPollEvents();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(gg_MainWindow);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(gg_MainWindow);
}

GLint Window::GetBufferWidth() const
{
	return gi_BufferWidth;
}
GLint Window::GetBufferHeight() const
{
	return gi_BufferHeight;
}

void Window::GetMousePos(float& pf_MouseX, float& pf_MouseY) const
{
	pf_MouseX = gf_MousePosX;
	pf_MouseY = gf_MousePosY;
}

const bool* Window::GetKeys() const
{
	return garr_Keys;
}

Window::~Window()
{
	glfwDestroyWindow(gg_MainWindow);
	glfwTerminate();
}

void Window::KeyboardHandeler(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	theWindow->gb_KeyHandlerTriggerd = true;


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key < 0 || key > KEY_COUNT)
		return;

	if (action == GLFW_PRESS)
		theWindow->garr_Keys[key] = true;
	else
	if (action == GLFW_RELEASE)
		theWindow->garr_Keys[key] = false;
}

void Window::MouseHandler(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	theWindow->gb_MouseHandlerTriggerd = true;

	theWindow->gf_MousePosX = float(xPos);
	theWindow->gf_MousePosY = float(yPos);
}

void Window::MouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	theWindow->gb_MouseButtonHandlerTriggerd = true;

	if (action == GLFW_PRESS)
		theWindow->garr_Keys[button] = true;
	else
	if (action == GLFW_RELEASE)
		theWindow->garr_Keys[button] = false;
}

void Window::WindowResizeHandler(GLFWwindow* window, int width, int height)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	theWindow->gb_WindowResizeHandlerTriggerd = true;

	theWindow->gi_BufferHeight = height;
	theWindow->gi_BufferWidth = width;
}

bool Window::GetKeyHandlerTriggerd()
{
	if (gb_KeyHandlerTriggerd)
	{
		gb_KeyHandlerTriggerd = false;
		return true;
	}
	return false;
}

bool Window::GetMouseHandlerTriggerd()
{
	if (gb_MouseHandlerTriggerd)
	{
		gb_MouseHandlerTriggerd = false;
		return true;
	}
	return false;
}

bool Window::GetMouseButtonHandlerTriggerd()
{
	if (gb_MouseButtonHandlerTriggerd)
	{
		gb_MouseButtonHandlerTriggerd = false;
		return true;
	}
	return false;
}

bool Window::GetWindowResizeHandlerTriggerd()
{
	if (gb_WindowResizeHandlerTriggerd)
	{
		gb_WindowResizeHandlerTriggerd = false;
		return true;
	}
	return false;
}

void Window::SetBorder(bool Border)
{
	if (Border)
		glfwSetWindowAttrib(gg_MainWindow, GLFW_DECORATED, GLFW_TRUE);
	else
		glfwSetWindowAttrib(gg_MainWindow, GLFW_DECORATED, GLFW_FALSE);
}

bool Window::GetBorder() const
{
	return glfwGetWindowAttrib(gg_MainWindow, GLFW_DECORATED) == GLFW_TRUE;
}

void Window::SetFullscreen(bool pb_Fullscreen)
{
	if (pb_Fullscreen)
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
		glfwSetWindowMonitor(gg_MainWindow, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		WindowResizeHandler(gg_MainWindow, mode->width, mode->height);
	}
	else
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
		glfwSetWindowMonitor(gg_MainWindow, nullptr, mode->width/2 - gi_Width/2, mode->height/2 - gi_Height/2, gi_Width, gi_Height, 0);
		WindowResizeHandler(gg_MainWindow, gi_Width, gi_Height);
	}

}

bool Window::GetFullscreen() const
{
	GLFWmonitor* currentMonitor = glfwGetWindowMonitor(gg_MainWindow);
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	return (currentMonitor == primaryMonitor);
}

bool Window::GetFocused() const
{
	return gb_Focused;
}

GLFWwindow* Window::GetWindow() const
{
	return gg_MainWindow;
}

void Window::FocusHandler(GLFWwindow* window, int focused) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	gb_Focused = focused;
}
