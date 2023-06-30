#pragma once
#include "DrawState.hpp"
#include "Loader.hpp"
#include "InputEvents.hpp"
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

private:
	IWindowImpl* _impl;
	std::vector<View*> _views;
	std::unique_ptr<Loader> _loader;

	InputEvents::MouseButtonCallback _mouseButtonCallback;
	InputEvents::MouseButtonWithMoveCallback _mouseButtonWithMoveCallback;
	InputEvents::MouseMoveCallback _mouseMoveCallback;
	InputEvents::MouseScrollCallback _mouseScrollCallback;
	InputEvents::KeyCallback _keyCallback;
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
	void setMouseButtonCallback(const InputEvents::MouseButtonCallback& callback) { _mouseButtonCallback = callback; }
	void setMouseButtonWithMoveCallback(const InputEvents::MouseButtonWithMoveCallback& callback) { _mouseButtonWithMoveCallback = callback; }
	void setMouseMoveCallback(const InputEvents::MouseMoveCallback& callback) { _mouseMoveCallback = callback; }
	void setMouseScrollCallback(const InputEvents::MouseScrollCallback& callback) { _mouseScrollCallback = callback; }
	void setKeyCallback(const InputEvents::KeyCallback& callback) { _keyCallback = callback; }
	void captureMouse();
	void uncaptureMouse();

private:
	Window(IWindowImpl* impl);
};
