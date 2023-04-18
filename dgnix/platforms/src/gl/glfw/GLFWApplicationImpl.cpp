#include "GLFWApplicationImpl.hpp"
#include "GLFWWindowImpl.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"
#include <assert.h>

bool GLFWApplicationImpl::init()
{
	return glfwInit() == GLFW_TRUE;
}

void GLFWApplicationImpl::terminate()
{
	glfwTerminate();
}

IWindowImpl* GLFWApplicationImpl::createWindow(int width, int height, const std::string& title)
{
	auto window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
	assert(didLoad);
	if (!didLoad) {
		glfwDestroyWindow(window);
		fprintf(stderr, "OpenGl functions isn't available!\n");
		return nullptr;
	}

	glfwMakeContextCurrent(NULL);

	printf("Number of functions that failed to load: %i.\n", didLoad.GetNumMissing());

	return new GLFWWindowImpl(window);
}