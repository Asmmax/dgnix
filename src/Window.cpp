#include "Window.hpp"
#include "IWindowImpl.hpp"
#include "IInputHandler.hpp"
#include "IGraphicsContext.hpp"
#include "IViewImpl.hpp"
#include "Loader.hpp"
#include "View.hpp"
#include "Model.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

void Window::captureMouse()
{
	if (auto inputHandler = _impl->getInputHandler()) {
		inputHandler->captureMouse();
	}
}

void Window::uncaptureMouse()
{
	if (auto inputHandler = _impl->getInputHandler()) {
		inputHandler->uncaptureMouse();
	}
}

Window::Window(IWindowImpl* impl):
	_impl(impl),
	_mainView(nullptr)
{
	_impl->init();

	if (auto inputHandler = _impl->getInputHandler()) {
		inputHandler->setMouseButtonCallback([this](InputEvents::MouseKey key, InputEvents::KeyState state) {
			if (_mouseButtonCallback) {
				_mouseButtonCallback(key, state);
			}
			});
		inputHandler->setMouseButtonWithMoveCallback([this](double x, double y, InputEvents::MouseKey key, InputEvents::KeyState state) {
			if (_mouseButtonWithMoveCallback) {
				_mouseButtonWithMoveCallback(x, y, key, state);
			}
			});
		inputHandler->setMouseMoveCallback([this](double x, double y) {
			if (_mouseMoveCallback) {
				_mouseMoveCallback(x, y);
			}
			});
		inputHandler->setMouseScrollCallback([this](double yOffset) {
			if (_mouseScrollCallback) {
				_mouseScrollCallback(yOffset);
			}
			});
		inputHandler->setKeyCallback([this](InputEvents::KeyboardKey key, InputEvents::KeyState state, InputEvents::KeyMod mod) {
			if (_keyCallback) {
				_keyCallback(key, state, mod);
			}
			});
		inputHandler->setFocusCallback([this](bool hasFocused) {
			if (_focusCallback) {
				_focusCallback(hasFocused);
			}
			});
	}
}

Window::~Window()
{
	for (View* viewPtr : _views) {
		delete viewPtr;
	}

	_loader.reset();

	_impl->terminate();

	delete _impl;
}

bool Window::isDone() const
{
	return _impl->isDone();
}

void Window::handle()
{
	if (_preHandleCallback) {
		_preHandleCallback();
	}
	_impl->handle();
}

View* Window::creteView(Texture* fboTexture)
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return nullptr;
	}

	IViewImpl* viewImpl = _impl->createView();
	if (!viewImpl) {
		return nullptr;
	}

	View* newView = new View(viewImpl, fboTexture);
	newView->init(graphicsContext);
	_views.push_back(newView);
	return newView;
}

void Window::releaseView(const View* view)
{
	if (_mainView == view) {
		_mainView = nullptr;
	}

	const auto foundIt = std::find(_views.begin(), _views.end(), view);
	if (foundIt != _views.end()) {
		delete* foundIt;
		_views.erase(foundIt);
	}
}

void Window::setMainView(View* view)
{
	assert(std::find(_views.begin(), _views.end(), view) != _views.end());
	_mainView = view;
}

Loader* Window::getLoader()
{
	if (!_loader) {
		auto graphicsContext = _impl->getGraphicsContext();
		if (!graphicsContext) {
			return nullptr;
		}

		auto loaderImpl = graphicsContext->createLoader();
		if (!loaderImpl) {
			return nullptr;
		}

		_loader = std::make_unique<Loader>(loaderImpl);
		_loader->init(graphicsContext);
	}
	return _loader.get();
}

void Window::getFrameSize(int& width, int& height) const
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}
	graphicsContext->getFrameSize(width, height);
}

void Window::render()
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}
	graphicsContext->makeCurrent();

	int frameWidth = 0;
	int frameHeight = 0;
	graphicsContext->getFrameSize(frameWidth, frameHeight);
	if (frameWidth == 0 || frameHeight == 0) {
		return;
	}

	if (!_mainView) {
		graphicsContext->setViewport(frameWidth, frameHeight);
		graphicsContext->clear(glm::vec3(0.0));
		return;
	}

	_mainView->copyBuffer(BufferId(), frameWidth, frameHeight);
}

void Window::setVSync(bool enabled)
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}
	graphicsContext->swapInterval(enabled ? 1 : 0);
}

void Window::setFullscreen(int width, int height, int framerate)
{
	if (_impl) {
		_impl->setFullscreen(width, height, framerate);
	}
}

int Window::getFramerate() const
{
	if (_impl) {
		return _impl->getFramerate();
	}
	return 0;
}

void Window::setWindowed()
{
	if (_impl) {
		_impl->setWindowed();
	}
}

void Window::swapBuffers()
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}
	graphicsContext->swapBuffers();
}
