#include "GLFWGraphicsContext.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../common/GLLoaderImpl.hpp"

GLFWGraphicsContext::GLFWGraphicsContext(GLFWwindow* window):
	_window(window),
	_imguiContext(nullptr)
{
}

void GLFWGraphicsContext::init()
{
	makeCurrent();

	IMGUI_CHECKVERSION();
	_imguiContext = ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init();

	gl::Enable(gl::DEPTH_TEST);
}

void GLFWGraphicsContext::terminate()
{
	makeCurrent();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(_imguiContext);
}

void GLFWGraphicsContext::makeCurrent()
{
	if (glfwGetCurrentContext() != _window) {
		glfwMakeContextCurrent(_window);
	}

	if (ImGui::GetCurrentContext() != _imguiContext) {
		ImGui::SetCurrentContext(_imguiContext);
	}
}

void GLFWGraphicsContext::getFrameSize(int& width, int& height) const
{
	glfwGetFramebufferSize(_window, &width, &height);
}

void GLFWGraphicsContext::setViewport(int width, int height)
{
	gl::Viewport(0, 0, width, height);
}

void GLFWGraphicsContext::clear(const glm::vec3& background, bool withDepth)
{
	gl::ClearColor(background.r, background.g, background.b, 1.0f);
	GLbitfield mask = gl::COLOR_BUFFER_BIT;
	if (withDepth) {
		mask |= gl::DEPTH_BUFFER_BIT;
	}
	gl::Clear(mask);
}

void GLFWGraphicsContext::clearDepth()
{
	gl::Clear(gl::DEPTH_BUFFER_BIT);
}

void GLFWGraphicsContext::setupImgui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
}

void GLFWGraphicsContext::renderImgui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLFWGraphicsContext::copyBuffer(BufferId srcId, int srcWidth, int srcHeight, BufferId dstId, int dstWidth, int dstHeight)
{
	gl::BindFramebuffer(gl::READ_FRAMEBUFFER, srcId);
	gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, dstId);

	gl::BlitFramebuffer(
		0, 0, srcWidth, srcHeight,
		0, 0, dstWidth, dstHeight,
		gl::COLOR_BUFFER_BIT,
		gl::NEAREST
	);

	gl::BindFramebuffer(gl::READ_FRAMEBUFFER, 0);
	gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);
}

void GLFWGraphicsContext::swapInterval(int frameCount)
{
	glfwSwapInterval(frameCount);
}

void GLFWGraphicsContext::swapBuffers()
{
	glfwSwapBuffers(_window);
}

ILoaderImpl* GLFWGraphicsContext::createLoader()
{
	return new GLLoaderImpl();
}
