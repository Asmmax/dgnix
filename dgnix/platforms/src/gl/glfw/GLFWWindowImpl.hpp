#pragma once
#include "IWindowImpl.hpp"
#include "GLFWGraphicsContext.hpp"
#include "GLFWInputHandler.hpp"
#include "../common/GLViewImpl.hpp"
#include <vector>

struct GLFWwindow;

class GLFWWindowImpl : public IWindowImpl
{
private:
	GLFWwindow* _window;
	GLFWGraphicsContext _graphicsContext;
	GLFWInputHandler _inputHandler;

	int _lastX;
	int _lastY;
	int _lastWidth;
	int _lastHeight;

public:
	GLFWWindowImpl(GLFWwindow* window);

	void init() override;
	void terminate() override;
	bool isDone() override;
	void handle() override;
	IGraphicsContext* getGraphicsContext() override;
	IInputHandler* getInputHandler() override;

	IViewImpl* createView() override;
	void setFullscreen(int width, int height, int framerate) override;
	int getFramerate() const override;
	void setWindowed() override;
};