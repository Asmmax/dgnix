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

void GLFWInputHandler::captureMouse()
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void GLFWInputHandler::uncaptureMouse()
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

void GLFWInputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
	auto* handler = static_cast<GLFWInputHandler*>(glfwGetWindowUserPointer(window));
	if (!handler) {
		return;
	}

	static std::vector<InputEvents::MouseKey> mouseKeyMap = {
		InputEvents::MouseKey::LEFT_BUTTON,
		InputEvents::MouseKey::RIGHT_BUTTON,
		InputEvents::MouseKey::MIDDLE_BUTTON,
		InputEvents::MouseKey::UNDEFINED,
		InputEvents::MouseKey::UNDEFINED,
		InputEvents::MouseKey::UNDEFINED,
		InputEvents::MouseKey::UNDEFINED,
		InputEvents::MouseKey::UNDEFINED
	};

	const InputEvents::MouseKey key = mouseKeyMap[button];

	static std::vector<InputEvents::KeyState> mouseKeyState = {
		InputEvents::KeyState::KEY_UP,
		InputEvents::KeyState::KEY_DOWN,
		InputEvents::KeyState::UNDEFINED
	};

	const InputEvents::KeyState state = mouseKeyState[action];

	if (handler->_mouseButtonCallback) {
		handler->_mouseButtonCallback(key, state);
	}

	double posX = 0, posY = 0;
	glfwGetCursorPos(window, &posX, &posY);
	if (handler->_mouseButtonWithMoveCallback) {
		handler->_mouseButtonWithMoveCallback(posX, posY, key, state);
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
