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
	ISize getFramebufferSize() override;

	void resizeBuffer() override;
	void clearBuffer(const glm::vec3& background) override;
	void setupImgui() override;
	void renderImgui() override;
	void swapInterval(int frameCount) override;
	void swapBuffers() override;

	ILoaderImpl* createLoader() override;
};