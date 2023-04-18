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

private:
	using MousePosCallback = std::function<void(double, double)>;
	using MouseScrollCallback = std::function<void(double)>;

	IWindowImpl* _impl;
	std::vector<View*> _views;
	std::unique_ptr<Loader> _loader;

	MousePosCallback _resetMousePosCallback;
	MousePosCallback _moveMouseCallback;
	MouseScrollCallback _scrollMouseCallback;

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
	void render(Model* model);
	void setupImgui();
	void renderImgui();
	void endRender();

	void setViewMatrix(const glm::mat4& matrix) { _viewMatrix = matrix; }
	const glm::mat4& getViewMatrix() const { return _viewMatrix; }

	void setBackground(const glm::vec3& color) { _background = color; }
	const glm::vec3& getBackground() const { return _background; }

	void setResetMousePosCallback(const MousePosCallback& callback) { _resetMousePosCallback = callback; }
	void setMoveMouseCallback(const MousePosCallback& callback) { _moveMouseCallback = callback; }
	void setScrollMouseCallback(const MouseScrollCallback& callback) { _scrollMouseCallback = callback; }

private:
	Window(IWindowImpl* impl);

	void mouseButtonCallback(double x, double y);
	void mousePositionCallback(double x, double y);
	void mouseScrollCallback(double yOffset);
};