#pragma once
#include "IInputHandler.hpp"

struct GLFWwindow;

class GLFWInputHandler : public IInputHandler
{
private:
	GLFWwindow* _window;
	InputEvents::MouseButtonCallback _mouseButtonCallback;
	InputEvents::MouseButtonWithMoveCallback _mouseButtonWithMoveCallback;
	InputEvents::MouseMoveCallback _mouseMoveCallback;
	InputEvents::MouseScrollCallback _mouseScrollCallback;

public:
	GLFWInputHandler(GLFWwindow* window);

	void init() override;
	void setMouseButtonCallback(const InputEvents::MouseButtonCallback& callback) override { _mouseButtonCallback = callback; }
	void setMouseButtonWithMoveCallback(const InputEvents::MouseButtonWithMoveCallback& callback) override { _mouseButtonWithMoveCallback = callback; }
	void setMouseMoveCallback(const InputEvents::MouseMoveCallback& callback) override { _mouseMoveCallback = callback; }
	void setMouseScrollCallback(const InputEvents::MouseScrollCallback& callback) override { _mouseScrollCallback = callback; }

	void captureMouse() override;
	void uncaptureMouse() override;

private:
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);
	static void mousePositionCallback(GLFWwindow* window, double x, double y);
	static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};
