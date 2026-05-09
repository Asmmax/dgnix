#include "View.hpp"
#include "Model.hpp"
#include "DrawState.hpp"
#include "RenderData.hpp"
#include "IGraphicsContext.hpp"
#include "IViewImpl.hpp"
#include "resources/Texture.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

static const StringId VIEW_MATRIX_NAME("ViewMatrix");
static const StringId PROJ_MATRIX_NAME("ProjectionMatrix");
static const StringId VIEW_PROJ_MATRIX_NAME("ViewProjectionMatrix");
static const StringId VIEW_ORIGIN_NAME("ViewOrigin");

View::View(IViewImpl* viewImpl, Texture* fboTexture):
	_impl(viewImpl),
	_context(nullptr),
	_fboTexture(fboTexture),
	_width(fboTexture ? fboTexture->getWidth() : 0),
	_height(fboTexture ? fboTexture->getHeight() : 0),
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

	if (model) {
		RenderData frameData;

		frameData.setMat4(VIEW_MATRIX_NAME, viewMatrix);
		frameData.setMat4(PROJ_MATRIX_NAME, projMatrix);

		const glm::mat3 viewMatrix3x3 = viewMatrix;
		const glm::vec3 viewOrigin = -glm::inverse(viewMatrix3x3) * viewMatrix[3];
		frameData.setVec3(VIEW_ORIGIN_NAME, viewOrigin);

		const glm::mat4 viewProjMat = projMatrix * viewMatrix;
		frameData.setMat4(VIEW_PROJ_MATRIX_NAME, viewProjMat);

		model->getState().fill(frameData);

		model->render(_renderQueue, viewProjMat);

		_renderQueue.sort();
		_renderQueue.execute(frameData);
		_renderQueue.clear();
	}

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
