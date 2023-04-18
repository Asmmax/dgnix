#include "GLFWInputHandler.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"

GLFWInputHandler::GLFWInputHandler(GLFWwindow* window):
	_window(window),
	_isRightMousePressed(false),
	_isLeftMousePressed(false)
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

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

			handler->_isRightMousePressed = true;

			double posX = 0, posY = 0;
			glfwGetCursorPos(window, &posX, &posY);
			if (handler->_mouseRightButtonDownCallback) {
				handler->_mouseRightButtonDownCallback(posX, posY);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

			handler->_isRightMousePressed = false;

			double posX = 0, posY = 0;
			glfwGetCursorPos(window, &posX, &posY);
			if (handler->_mouseRightButtonUpCallback) {
				handler->_mouseRightButtonUpCallback(posX, posY);
			}
		}
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
