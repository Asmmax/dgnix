#pragma once
#include "DrawState.hpp"
#include "Loader.hpp"
#include <memory>
#include <vector>
#include <functional>

class IWindowImpl;
class Texture;
class Model;
class View;

class Window
{
	friend class Application;

public:
	using VoidCallback = std::function<void()>;
	using MouseButtonCallback = std::function<void(double, double)>;
	using MouseMoveCallback = std::function<void(double, double)>;
	using MouseScrollCallback = std::function<void(double)>;

private:
	IWindowImpl* _impl;
	std::vector<View*> _views;
	std::unique_ptr<Loader> _loader;

	MouseButtonCallback _mouseRightButtonDownCallback;
	MouseButtonCallback _mouseRightButtonUpCallback;
	MouseButtonCallback _mouseLeftButtonDownCallback;
	MouseButtonCallback _mouseLeftButtonUpCallback;
	MouseMoveCallback _mouseMoveCallback;
	MouseScrollCallback _mouseScrollCallback;
	VoidCallback _preHandleCallback;

	glm::vec3 _background;
	glm::mat4 _viewMatrix;
	DrawStatePoolDef _statePool;

public:
	~Window();

	bool isDone();
	void handle();
	View* creteView(Texture* fboTexture);
	Loader* getLoader();

	void beginRender();
	void render(const Model* model);
	void setupImgui();
	void renderImgui();
	void endRender();

	void setViewMatrix(const glm::mat4& matrix) { _viewMatrix = matrix; }
	const glm::mat4& getViewMatrix() const { return _viewMatrix; }

	void setBackground(const glm::vec3& color) { _background = color; }
	const glm::vec3& getBackground() const { return _background; }

	void setPreHandleCallback(const VoidCallback& callback) { _preHandleCallback = callback; }
	void setMouseRightButtonDownCallback(const MouseButtonCallback& callback) { _mouseRightButtonDownCallback = callback; }
	void setMouseRightButtonUpCallback(const MouseButtonCallback& callback) { _mouseRightButtonUpCallback = callback; }
	void setMouseLeftButtonDownCallback(const MouseButtonCallback& callback) { _mouseLeftButtonDownCallback = callback; }
	void setMouseLeftButtonUpCallback(const MouseButtonCallback& callback) { _mouseLeftButtonUpCallback = callback; }
	void setMouseMoveCallback(const MouseMoveCallback& callback) { _mouseMoveCallback = callback; }
	void setMouseScrollCallback(const MouseScrollCallback& callback) { _mouseScrollCallback = callback; }

private:
	Window(IWindowImpl* impl);
};