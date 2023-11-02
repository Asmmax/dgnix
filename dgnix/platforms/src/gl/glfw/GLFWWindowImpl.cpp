#include "GLFWWindowImpl.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"

GLFWWindowImpl::GLFWWindowImpl(GLFWwindow* window):
	_window(window),
	_graphicsContext(_window),
	_inputHandler(_window),
	_lastX(0),
	_lastY(0),
	_lastWidth(0),
	_lastHeight(0)
{
}

void GLFWWindowImpl::init()
{
	_inputHandler.init();
	_graphicsContext.init();

	ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);
}

void GLFWWindowImpl::terminate()
{
	_graphicsContext.terminate();
	glfwDestroyWindow(_window);
}

bool GLFWWindowImpl::isDone()
{
	return glfwWindowShouldClose(_window) != 0;
}

void GLFWWindowImpl::handle()
{
	glfwPollEvents();
}

IGraphicsContext* GLFWWindowImpl::getGraphicsContext()
{
	return &_graphicsContext;
}

IInputHandler* GLFWWindowImpl::getInputHandler()
{
	return &_inputHandler;
}

IViewImpl* GLFWWindowImpl::createView()
{
	return new GLViewImpl();
}

void GLFWWindowImpl::setFullscreen(int width, int height, int framerate)
{
	GLFWmonitor* currentMonitor = glfwGetWindowMonitor(_window);
	if (currentMonitor != NULL) {
		return;
	}

	glfwGetWindowPos(_window, &_lastX, &_lastY);
	glfwGetWindowSize(_window, &_lastWidth, &_lastHeight);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	const int selWidth = width <= 0 ? mode->width : width;
	const int selHeight = height <= 0 ? mode->height : height;
	const int refreshRate = framerate <= 0 ? mode->refreshRate : framerate;
	glfwSetWindowMonitor(_window, monitor, 0, 0, selWidth, selHeight, refreshRate);
}

int GLFWWindowImpl::getFramerate() const
{
	GLFWmonitor* currentMonitor = glfwGetWindowMonitor(_window);
	if (currentMonitor == NULL) {
		return 0;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(currentMonitor);
	return mode->refreshRate;
}

void GLFWWindowImpl::setWindowed()
{
	GLFWmonitor* currentMonitor = glfwGetWindowMonitor(_window);
	if (currentMonitor != NULL) {
		glfwSetWindowMonitor(_window, NULL, _lastX, _lastY, _lastWidth, _lastHeight, GLFW_DONT_CARE);
	}
}
