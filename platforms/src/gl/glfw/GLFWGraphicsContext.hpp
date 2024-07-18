#pragma once
#include "IGraphicsContext.hpp"

struct GLFWwindow;
struct ImGuiContext;

class GLFWGraphicsContext : public IGraphicsContext
{
private:
	GLFWwindow* _window;
	ImGuiContext* _imguiContext;

public:
	explicit GLFWGraphicsContext(GLFWwindow* window);

	void init() override;
	void terminate() override;
	void makeCurrent() override;
	void getFrameSize(int& width, int& height) const override;

	void setViewport(int width, int height) override;
	void clear(const glm::vec3& background, bool withDepth) override;
	void clearDepth() override;
	void setupImgui() override;
	void renderImgui() override;
	void copyBuffer(BufferId srcId, int srcWidth, int srcHeight, BufferId dstId, int dstWidth, int dstHeight) override;
	void swapInterval(int frameCount) override;
	void swapBuffers() override;

	ILoaderImpl* createLoader() override;
};