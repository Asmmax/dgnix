#pragma once
#include "DrawState.hpp"
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Model;
class IGraphicsContext;
class IViewImpl;
class Texture;

class View
{
private:
	IViewImpl* _impl;
	IGraphicsContext* _context;
	Texture* _fboTexture;
	int _width;
	int _height;
	glm::vec3 _background;
	glm::mat4 _matrix;
	DrawStatePoolDef _statePool;

	bool _isResized;

public:
	View(IViewImpl* viewImpl, Texture* fboTexture);
	~View();

	void init(IGraphicsContext* context);
	void render(Model* model);
	void setSize(int width, int height);

	void setMatrix(const glm::mat4& matrix) { _matrix = matrix; }
	const glm::mat4& getMatrix() const { return _matrix; }

	Texture* getFboTexture() const { return _fboTexture; }

	void setBackground(const glm::vec3& color) { _background = color; }
	const glm::vec3& getBackground() const { return _background; }

private:
	void resizeBuffer();
};
