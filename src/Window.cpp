#include "Window.hpp"
#include "IWindowImpl.hpp"
#include "IInputHandler.hpp"
#include "IGraphicsContext.hpp"
#include "IViewImpl.hpp"
#include "Loader.hpp"
#include "View.hpp"
#include "Model.hpp"
#include "imgui.h"
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
	_background(0.0f, 0.0f, 0.0f),
	_viewMatrix(),
	_statePool(10)
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
	const auto foundIt = std::find(_views.begin(), _views.end(), view);
	if (foundIt != _views.end()) {
		delete* foundIt;
		_views.erase(foundIt);
	}
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

void Window::prepareRender()
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}
	graphicsContext->makeCurrent();
	graphicsContext->resizeBuffer();
	graphicsContext->clearBuffer(_background);
}

void Window::render(const Model* model)
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}

	const auto size = graphicsContext->getFramebufferSize();
	if (size.width == 0 || size.height == 0) {
		return;
	}

	_statePool.push();

	const glm::mat4 projMat = glm::perspective(45.0f, size.width / (float)size.height, 0.01f, 1000.0f);

	if (model) {
		model->predraw(_statePool, _viewMatrix, projMat);
		model->draw(_statePool);
	}

	_statePool.pop();
}

void Window::setupImgui()
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}

	graphicsContext->setupImgui();
	ImGui::NewFrame();
}

void Window::renderImgui()
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}

	ImGui::Render();
	graphicsContext->renderImgui();
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
