#pragma once
#include "IInputHandler.hpp"

struct GLFWwindow;

class GLFWInputHandler : public IInputHandler
{
private:
	GLFWwindow* _window;
	MouseButtonCallback _mouseRightButtonDownCallback;
	MouseButtonCallback _mouseRightButtonUpCallback;
	MouseButtonCallback _mouseLeftButtonDownCallback;
	MouseButtonCallback _mouseLeftButtonUpCallback;
	MouseMoveCallback _mouseMoveCallback;
	MouseScrollCallback _mouseScrollCallback;

public:
	GLFWInputHandler(GLFWwindow* window);

	void init() override;
	void setMouseRightButtonDownCallback(const MouseButtonCallback& callback) override { _mouseRightButtonDownCallback = callback; }
	void setMouseRightButtonUpCallback(const MouseButtonCallback& callback) override { _mouseRightButtonUpCallback = callback; }
	void setMouseLeftButtonDownCallback(const MouseButtonCallback& callback) override { _mouseLeftButtonDownCallback = callback; }
	void setMouseLeftButtonUpCallback(const MouseButtonCallback& callback) override { _mouseLeftButtonUpCallback = callback; }
	void setMouseMoveCallback(const MouseMoveCallback& callback) override { _mouseMoveCallback = callback; }
	void setMouseScrollCallback(const MouseScrollCallback& callback) override { _mouseScrollCallback = callback; }

private:
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);
	static void mousePositionCallback(GLFWwindow* window, double x, double y);
	static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	static void captureMouse(GLFWwindow* window);
	static void uncaptureMouse(GLFWwindow* window);
};
