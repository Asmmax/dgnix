#include "View.hpp"
#include "Model.hpp"
#include "DrawState.hpp"
#include "IGraphicsContext.hpp"
#include "IViewImpl.hpp"
#include "resources/Texture.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

View::View(IViewImpl* viewImpl, Texture* fboTexture):
	_impl(viewImpl),
	_context(nullptr),
	_fboTexture(fboTexture),
	_width(fboTexture ? fboTexture->getWidth() : 0),
	_height(fboTexture ? fboTexture->getHeight() : 0),
	_statePool(10),
	_isResized(false),
	_isRendering(false)
{
}

View::~View()
{
	_impl->terminate();
	delete _impl;
}

void View::init(IGraphicsContext* context)
{
	_context = context;

	if (!_context) {
		return;
	}
	_context->makeCurrent();

	_impl->init(_fboTexture->getId());
	_impl->resizeBuffer(_width, _height);
}

void View::beginRender(const glm::vec3& background)
{
	assert(!_isRendering);
	if (!_context) {
		return;
	}
	_context->makeCurrent();

	resizeBuffer();

	_impl->beginDraw();

	_context->setViewport(_width, _height);
	_context->clear(background);

	_isRendering = true;
}

void View::render(const Model* model, const glm::mat4& viewMatrix, const glm::mat4& projMatrix, bool isBackground)
{
	assert(_isRendering);
	if (_width == 0 || _height == 0) {
		return;
	}

	if (!_context) {
		return;
	}

	_statePool.push();

	if (model) {
		model->predraw(_statePool, viewMatrix, projMatrix);
		model->draw(_statePool);
	}

	_statePool.pop();

	if (isBackground) {
		_context->clearDepth();
	}
}

void View::setupImgui()
{
	assert(_isRendering);
	if (!_context) {
		return;
	}

	_context->setupImgui();
	ImGui::NewFrame();
}

void View::renderImgui()
{
	assert(_isRendering);
	if (!_context) {
		return;
	}

	ImGui::Render();
	_context->renderImgui();
}

void View::endRender()
{
	assert(_isRendering);

	if (!_context) {
		return;
	}

	_impl->endDraw();

	_isRendering = false;
}

void View::setSize(int width, int height)
{
	if (_width != width || _height != height) {
		_width = width;
		_height = height;
		_isResized = true;
	}
}

void View::copyBuffer(BufferId dstId, int dstWidth, int dstHeight)
{
	BufferId fboId = _impl->getFrameBufferId();
	if (!fboId || !_fboTexture) {
		return;
	}

	if (!_context) {
		return;
	}
	_context->makeCurrent();
	_context->copyBuffer(fboId, _fboTexture->getWidth(), _fboTexture->getHeight(), dstId, dstWidth, dstHeight);
}

void View::resizeBuffer()
{
	if (!_isResized) {
		return;
	}

	_fboTexture->resize(_width, _height);
	_impl->resizeBuffer(_width, _height);

	_isResized = false;
}
