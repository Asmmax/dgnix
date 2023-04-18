#include "GLFWWindowImpl.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"

GLFWWindowImpl::GLFWWindowImpl(GLFWwindow* window):
	_window(window),
	_graphicsContext(_window),
	_inputHandler(_window)
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
