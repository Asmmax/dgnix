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
	View* _mainView;

	InputEvents::MouseButtonCallback _mouseButtonCallback;
	InputEvents::MouseButtonWithMoveCallback _mouseButtonWithMoveCallback;
	InputEvents::MouseMoveCallback _mouseMoveCallback;
	InputEvents::MouseScrollCallback _mouseScrollCallback;
	InputEvents::KeyCallback _keyCallback;
	InputEvents::FocusCallback _focusCallback;
	VoidCallback _preHandleCallback;

public:
	~Window();

	Window(const Window& other) = delete;
	Window(Window&& other) = delete;
	Window& operator=(const Window& other) = delete;
	Window& operator=(Window&& other) = delete;

	bool isDone() const;
	void handle();
	View* creteView(Texture* fboTexture);
	void releaseView(const View* view);
	void setMainView(View* view);
	bool isMainView(const View* view) const;
	Loader* getLoader();

	void getFrameSize(int& width, int& height) const;

	void render();
	void setVSync(bool enabled);
	void setFullscreen(int width, int height, int framerate);
	int getFramerate() const;
	void setWindowed();
	void swapBuffers();

	void setPreHandleCallback(const VoidCallback& callback) { _preHandleCallback = callback; }
	void setFocusCallback(const InputEvents::FocusCallback& callback) { _focusCallback = callback; }
	void setMouseButtonCallback(const InputEvents::MouseButtonCallback& callback) { _mouseButtonCallback = callback; }
	void setMouseButtonWithMoveCallback(const InputEvents::MouseButtonWithMoveCallback& callback) { _mouseButtonWithMoveCallback = callback; }
	void setMouseMoveCallback(const InputEvents::MouseMoveCallback& callback) { _mouseMoveCallback = callback; }
	void setMouseScrollCallback(const InputEvents::MouseScrollCallback& callback) { _mouseScrollCallback = callback; }
	void setKeyCallback(const InputEvents::KeyCallback& callback) { _keyCallback = callback; }
	void captureMouse();
	void uncaptureMouse();

private:
	explicit Window(IWindowImpl* impl);
};
