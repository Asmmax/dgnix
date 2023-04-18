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

Window::Window(IWindowImpl* impl):
	_impl(impl),
	_background(0.0f, 0.0f, 0.0f),
	_viewMatrix(),
	_statePool(10)
{
	_impl->init();

	if (auto inputHandler = _impl->getInputHandler()) {
		inputHandler->setMouseRightButtonDownCallback([this](double x, double y) {
			mouseButtonCallback(x, y);
			});
		inputHandler->setMouseMoveCallback([this](double x, double y) {
			mousePositionCallback(x, y);
			});
		inputHandler->setMouseScrollCallback([this](double yOffset) {
			mouseScrollCallback(yOffset);
			});
	}
}

Window::~Window()
{
	for (auto& viewPtr : _views) {
		delete viewPtr;
	}

	_loader.reset();

	_impl->terminate();

	delete _impl;
}

bool Window::isDone()
{
	return _impl->isDone();
}

void Window::handle()
{
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

void Window::beginRender()
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}
	graphicsContext->makeCurrent();
	graphicsContext->resizeBuffer();
	graphicsContext->clearBuffer(_background);
}

void Window::render(Model* model)
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}

	_statePool.push();

	auto size = graphicsContext->getFramebufferSize();
	glm::mat4 projMat = glm::perspective(45.0f, size.width / (float)size.height, 0.01f, 1000.0f);

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

void Window::endRender()
{
	auto graphicsContext = _impl->getGraphicsContext();
	if (!graphicsContext) {
		return;
	}
	graphicsContext->swapBuffers();
}

void Window::mouseButtonCallback(double posX, double posY)
{
	if (!_resetMousePosCallback) {
		return;
	}

	_resetMousePosCallback(posX, posY);
}

void Window::mousePositionCallback(double x, double y)
{
	if (!_moveMouseCallback) {
		return;
	}

	auto inputHandler = _impl->getInputHandler();
	if (!inputHandler) {
		return;
	}

	if (inputHandler->isRightMousePressed()) {
		_moveMouseCallback(x, y);
	}
}

void Window::mouseScrollCallback(double yOffset)
{
	if (!_scrollMouseCallback) {
		return;
	}

	_scrollMouseCallback(yOffset);
}
