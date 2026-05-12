#pragma once
#include "RenderQueue.hpp"
#include "BufferId.hpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Model;
class IGraphicsContext;
class IViewImpl;
class Texture;

class View
{
private:
	RenderQueue _renderQueue;
	IViewImpl* _impl;
	IGraphicsContext* _context;
	Texture* _fboTexture;
	glm::mat4 _projMatrix;
	int _width;
	int _height;
	float _fovy;
	float _nearDist;
	float _farDist;

	bool _isResized;
	bool _isProjChanged;
	bool _isRendering;

public:
	View(IViewImpl* viewImpl, Texture* fboTexture);
	~View();

	View(const View& other) = delete;
	View(View&& other) = delete;
	View& operator=(const View& other) = delete;
	View& operator=(View&& other) = delete;

	void init(IGraphicsContext* context);
	void beginRender(const glm::vec3& background = glm::vec3());
	void render(const Model* model, const glm::mat4& viewMatrix, bool isBackground = false);
	void render(const RenderQueue& renderQueue, const RenderData& modelData, const glm::mat4& viewMatrix, bool isBackground = false);
	void setupImgui();
	void renderImgui();
	void endRender();
	void setSize(int width, int height);
	void setFrustum(float fovy, float nearDist, float farDist);
	int getWidth() const { return _width; }
	int getHeight() const { return _height; }

	void copyBuffer(BufferId dstId, int dstWidth, int dstHeight);

	Texture* getFboTexture() const { return _fboTexture; }

private:
	void resizeBuffer();
	void updateProjMatrix();
};
