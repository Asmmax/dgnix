#include "GLFWInputHandler.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"
#include <unordered_map>

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
	glfwSetKeyCallback(_window, keyCallback);
	glfwSetWindowFocusCallback(_window, focusCallback);
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

void GLFWInputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto* handler = static_cast<GLFWInputHandler*>(glfwGetWindowUserPointer(window));
	if (!handler) {
		return;
	}

	static std::unordered_map<int, InputEvents::KeyboardKey> keyMap = {
		{GLFW_KEY_SPACE, InputEvents::KeyboardKey::KEY_SPACE },
		{GLFW_KEY_APOSTROPHE, InputEvents::KeyboardKey::KEY_APOSTROPHE },
		{GLFW_KEY_COMMA, InputEvents::KeyboardKey::KEY_COMMA },
		{GLFW_KEY_MINUS, InputEvents::KeyboardKey::KEY_MINUS },
		{GLFW_KEY_PERIOD, InputEvents::KeyboardKey::KEY_PERIOD },
		{GLFW_KEY_SLASH, InputEvents::KeyboardKey::KEY_SLASH },
		{GLFW_KEY_0, InputEvents::KeyboardKey::KEY_0 },
		{GLFW_KEY_1, InputEvents::KeyboardKey::KEY_1 },
		{GLFW_KEY_2, InputEvents::KeyboardKey::KEY_2 },
		{GLFW_KEY_3, InputEvents::KeyboardKey::KEY_3 },
		{GLFW_KEY_4, InputEvents::KeyboardKey::KEY_4 },
		{GLFW_KEY_5, InputEvents::KeyboardKey::KEY_5 },
		{GLFW_KEY_6, InputEvents::KeyboardKey::KEY_6 },
		{GLFW_KEY_7, InputEvents::KeyboardKey::KEY_7 },
		{GLFW_KEY_8, InputEvents::KeyboardKey::KEY_8 },
		{GLFW_KEY_9, InputEvents::KeyboardKey::KEY_9 },
		{GLFW_KEY_SEMICOLON, InputEvents::KeyboardKey::KEY_SEMICOLON },
		{GLFW_KEY_EQUAL, InputEvents::KeyboardKey::KEY_EQUAL },
		{GLFW_KEY_A, InputEvents::KeyboardKey::KEY_A },
		{GLFW_KEY_B, InputEvents::KeyboardKey::KEY_B },
		{GLFW_KEY_C, InputEvents::KeyboardKey::KEY_C },
		{GLFW_KEY_D, InputEvents::KeyboardKey::KEY_D },
		{GLFW_KEY_E, InputEvents::KeyboardKey::KEY_E },
		{GLFW_KEY_F, InputEvents::KeyboardKey::KEY_F },
		{GLFW_KEY_G, InputEvents::KeyboardKey::KEY_G },
		{GLFW_KEY_H, InputEvents::KeyboardKey::KEY_H },
		{GLFW_KEY_I, InputEvents::KeyboardKey::KEY_I },
		{GLFW_KEY_J, InputEvents::KeyboardKey::KEY_J },
		{GLFW_KEY_K, InputEvents::KeyboardKey::KEY_K },
		{GLFW_KEY_L, InputEvents::KeyboardKey::KEY_L },
		{GLFW_KEY_M, InputEvents::KeyboardKey::KEY_M },
		{GLFW_KEY_N, InputEvents::KeyboardKey::KEY_N },
		{GLFW_KEY_O, InputEvents::KeyboardKey::KEY_O },
		{GLFW_KEY_P, InputEvents::KeyboardKey::KEY_P },
		{GLFW_KEY_Q, InputEvents::KeyboardKey::KEY_Q },
		{GLFW_KEY_R, InputEvents::KeyboardKey::KEY_R },
		{GLFW_KEY_S, InputEvents::KeyboardKey::KEY_S },
		{GLFW_KEY_T, InputEvents::KeyboardKey::KEY_T },
		{GLFW_KEY_U, InputEvents::KeyboardKey::KEY_U },
		{GLFW_KEY_V, InputEvents::KeyboardKey::KEY_V },
		{GLFW_KEY_W, InputEvents::KeyboardKey::KEY_W },
		{GLFW_KEY_X, InputEvents::KeyboardKey::KEY_X },
		{GLFW_KEY_Y, InputEvents::KeyboardKey::KEY_Y },
		{GLFW_KEY_Z, InputEvents::KeyboardKey::KEY_Z },
		{GLFW_KEY_LEFT_BRACKET, InputEvents::KeyboardKey::KEY_LEFT_BRACKET },
		{GLFW_KEY_BACKSLASH, InputEvents::KeyboardKey::KEY_BACKSLASH },
		{GLFW_KEY_RIGHT_BRACKET, InputEvents::KeyboardKey::KEY_RIGHT_BRACKET },
		{GLFW_KEY_GRAVE_ACCENT, InputEvents::KeyboardKey::KEY_GRAVE_ACCENT },
		{GLFW_KEY_WORLD_1, InputEvents::KeyboardKey::KEY_WORLD_1 },
		{GLFW_KEY_WORLD_2, InputEvents::KeyboardKey::KEY_WORLD_2 },
		{GLFW_KEY_ESCAPE, InputEvents::KeyboardKey::KEY_ESCAPE },
		{GLFW_KEY_ENTER, InputEvents::KeyboardKey::KEY_ENTER },
		{GLFW_KEY_TAB, InputEvents::KeyboardKey::KEY_TAB },
		{GLFW_KEY_BACKSPACE, InputEvents::KeyboardKey::KEY_BACKSPACE },
		{GLFW_KEY_INSERT, InputEvents::KeyboardKey::KEY_INSERT },
		{GLFW_KEY_DELETE, InputEvents::KeyboardKey::KEY_DELETE },
		{GLFW_KEY_RIGHT, InputEvents::KeyboardKey::KEY_RIGHT },
		{GLFW_KEY_LEFT, InputEvents::KeyboardKey::KEY_LEFT },
		{GLFW_KEY_DOWN, InputEvents::KeyboardKey::KEY_DOWN },
		{GLFW_KEY_UP, InputEvents::KeyboardKey::KEY_UP },
		{GLFW_KEY_PAGE_UP, InputEvents::KeyboardKey::KEY_PAGE_UP },
		{GLFW_KEY_PAGE_DOWN, InputEvents::KeyboardKey::KEY_PAGE_DOWN },
		{GLFW_KEY_HOME, InputEvents::KeyboardKey::KEY_HOME },
		{GLFW_KEY_END, InputEvents::KeyboardKey::KEY_END },
		{GLFW_KEY_CAPS_LOCK, InputEvents::KeyboardKey::KEY_CAPS_LOCK },
		{GLFW_KEY_SCROLL_LOCK, InputEvents::KeyboardKey::KEY_SCROLL_LOCK },
		{GLFW_KEY_NUM_LOCK, InputEvents::KeyboardKey::KEY_NUM_LOCK },
		{GLFW_KEY_PRINT_SCREEN, InputEvents::KeyboardKey::KEY_PRINT_SCREEN },
		{GLFW_KEY_PAUSE, InputEvents::KeyboardKey::KEY_PAUSE },
		{GLFW_KEY_F1, InputEvents::KeyboardKey::KEY_F1 },
		{GLFW_KEY_F2, InputEvents::KeyboardKey::KEY_F2 },
		{GLFW_KEY_F3, InputEvents::KeyboardKey::KEY_F3 },
		{GLFW_KEY_F4, InputEvents::KeyboardKey::KEY_F4 },
		{GLFW_KEY_F5, InputEvents::KeyboardKey::KEY_F5 },
		{GLFW_KEY_F6, InputEvents::KeyboardKey::KEY_F6 },
		{GLFW_KEY_F7, InputEvents::KeyboardKey::KEY_F7 },
		{GLFW_KEY_F8, InputEvents::KeyboardKey::KEY_F8 },
		{GLFW_KEY_F9, InputEvents::KeyboardKey::KEY_F9 },
		{GLFW_KEY_F10, InputEvents::KeyboardKey::KEY_F10 },
		{GLFW_KEY_F11, InputEvents::KeyboardKey::KEY_F11 },
		{GLFW_KEY_F12, InputEvents::KeyboardKey::KEY_F12 },
		{GLFW_KEY_F13, InputEvents::KeyboardKey::KEY_F13 },
		{GLFW_KEY_F14, InputEvents::KeyboardKey::KEY_F14 },
		{GLFW_KEY_F15, InputEvents::KeyboardKey::KEY_F15 },
		{GLFW_KEY_F16, InputEvents::KeyboardKey::KEY_F16 },
		{GLFW_KEY_F17, InputEvents::KeyboardKey::KEY_F17 },
		{GLFW_KEY_F18, InputEvents::KeyboardKey::KEY_F18 },
		{GLFW_KEY_F19, InputEvents::KeyboardKey::KEY_F19 },
		{GLFW_KEY_F20, InputEvents::KeyboardKey::KEY_F20 },
		{GLFW_KEY_F21, InputEvents::KeyboardKey::KEY_F21 },
		{GLFW_KEY_F22, InputEvents::KeyboardKey::KEY_F22 },
		{GLFW_KEY_F23, InputEvents::KeyboardKey::KEY_F23 },
		{GLFW_KEY_F24, InputEvents::KeyboardKey::KEY_F24 },
		{GLFW_KEY_F25, InputEvents::KeyboardKey::KEY_F25 },
		{GLFW_KEY_KP_0, InputEvents::KeyboardKey::KEY_KP_0 },
		{GLFW_KEY_KP_1, InputEvents::KeyboardKey::KEY_KP_1 },
		{GLFW_KEY_KP_2, InputEvents::KeyboardKey::KEY_KP_2 },
		{GLFW_KEY_KP_3, InputEvents::KeyboardKey::KEY_KP_3 },
		{GLFW_KEY_KP_4, InputEvents::KeyboardKey::KEY_KP_4 },
		{GLFW_KEY_KP_5, InputEvents::KeyboardKey::KEY_KP_5 },
		{GLFW_KEY_KP_6, InputEvents::KeyboardKey::KEY_KP_6 },
		{GLFW_KEY_KP_7, InputEvents::KeyboardKey::KEY_KP_7 },
		{GLFW_KEY_KP_8, InputEvents::KeyboardKey::KEY_KP_8 },
		{GLFW_KEY_KP_9, InputEvents::KeyboardKey::KEY_KP_9 },
		{GLFW_KEY_KP_DECIMAL, InputEvents::KeyboardKey::KEY_KP_DECIMAL },
		{GLFW_KEY_KP_DIVIDE, InputEvents::KeyboardKey::KEY_KP_DIVIDE },
		{GLFW_KEY_KP_MULTIPLY, InputEvents::KeyboardKey::KEY_KP_MULTIPLY },
		{GLFW_KEY_KP_SUBTRACT, InputEvents::KeyboardKey::KEY_KP_SUBTRACT },
		{GLFW_KEY_KP_ADD, InputEvents::KeyboardKey::KEY_KP_ADD },
		{GLFW_KEY_KP_ENTER, InputEvents::KeyboardKey::KEY_KP_ENTER },
		{GLFW_KEY_KP_EQUAL, InputEvents::KeyboardKey::KEY_KP_EQUAL },
		{GLFW_KEY_LEFT_SHIFT, InputEvents::KeyboardKey::KEY_LEFT_SHIFT },
		{GLFW_KEY_LEFT_CONTROL, InputEvents::KeyboardKey::KEY_LEFT_CONTROL },
		{GLFW_KEY_LEFT_ALT, InputEvents::KeyboardKey::KEY_LEFT_ALT },
		{GLFW_KEY_LEFT_SUPER, InputEvents::KeyboardKey::KEY_LEFT_SUPER },
		{GLFW_KEY_RIGHT_SHIFT, InputEvents::KeyboardKey::KEY_RIGHT_SHIFT },
		{GLFW_KEY_RIGHT_CONTROL, InputEvents::KeyboardKey::KEY_RIGHT_CONTROL },
		{GLFW_KEY_RIGHT_ALT, InputEvents::KeyboardKey::KEY_RIGHT_ALT },
		{GLFW_KEY_RIGHT_SUPER, InputEvents::KeyboardKey::KEY_RIGHT_SUPER },
		{GLFW_KEY_MENU, InputEvents::KeyboardKey::KEY_MENU },
		{GLFW_KEY_LAST, InputEvents::KeyboardKey::KEY_MENU },
		{GLFW_KEY_UNKNOWN, InputEvents::KeyboardKey::UNDEFINED },
	};

	const InputEvents::KeyboardKey keyboardKey = keyMap[key];

	static std::vector<InputEvents::KeyState> keyState = {
		InputEvents::KeyState::KEY_UP,
		InputEvents::KeyState::KEY_DOWN,
		InputEvents::KeyState::UNDEFINED
	};

	const InputEvents::KeyState state = keyState[action];

	const InputEvents::KeyMod mod = static_cast<InputEvents::KeyMod>(mods & static_cast<int>(InputEvents::KeyMod::ALL));

	if (handler->_keyCallback) {
		handler->_keyCallback(keyboardKey, state, mod);
	}
}

void GLFWInputHandler::focusCallback(GLFWwindow* window, int focused)
{
	auto* handler = static_cast<GLFWInputHandler*>(glfwGetWindowUserPointer(window));
	if (handler->_focusCallback) {
		handler->_focusCallback(focused == GLFW_TRUE);
	}
}
