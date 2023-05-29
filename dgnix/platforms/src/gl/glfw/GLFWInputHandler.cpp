#include "GLFWInputHandler.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"

GLFWInputHandler::GLFWInputHandler(GLFWwindow* window):
	_window(window)
{
}

void GLFWInputHandler::init()
{
	glfwSetWindowUserPointer(_window, static_cast<void*>(this));

	glfwSetMouseButtonCallback(_window, mouseButtonCallback);
	glfwSetCursorPosCallback(_window, mousePositionCallback);
	glfwSetScrollCallback(_window, mouseScrollCallback);
}

void GLFWInputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
	auto* handler = static_cast<GLFWInputHandler*>(glfwGetWindowUserPointer(window));
	if (!handler) {
		return;
	}

	switch (button) 
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS) {
			captureMouse(window);

			double posX = 0, posY = 0;
			glfwGetCursorPos(window, &posX, &posY);
			if (handler->_mouseRightButtonDownCallback) {
				handler->_mouseRightButtonDownCallback(posX, posY);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			uncaptureMouse(window);

			double posX = 0, posY = 0;
			glfwGetCursorPos(window, &posX, &posY);
			if (handler->_mouseRightButtonUpCallback) {
				handler->_mouseRightButtonUpCallback(posX, posY);
			}
		}
		break;
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) {
			captureMouse(window);

			double posX = 0, posY = 0;
			glfwGetCursorPos(window, &posX, &posY);
			if (handler->_mouseLeftButtonDownCallback) {
				handler->_mouseLeftButtonDownCallback(posX, posY);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			uncaptureMouse(window);

			double posX = 0, posY = 0;
			glfwGetCursorPos(window, &posX, &posY);
			if (handler->_mouseLeftButtonUpCallback) {
				handler->_mouseLeftButtonUpCallback(posX, posY);
			}
		}
		break;
	default:
		break;
	}
}

void GLFWInputHandler::mousePositionCallback(GLFWwindow* window, double x, double y)
{
	auto* handler = static_cast<GLFWInputHandler*>(glfwGetWindowUserPointer(window));
	if (handler->_mouseMoveCallback) {
		handler->_mouseMoveCallback(x, y);
	}
}

void GLFWInputHandler::mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	auto* handler = static_cast<GLFWInputHandler*>(glfwGetWindowUserPointer(window));
	if (handler->_mouseScrollCallback) {
		handler->_mouseScrollCallback(yOffset);
	}
}

void GLFWInputHandler::captureMouse(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void GLFWInputHandler::uncaptureMouse(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}
