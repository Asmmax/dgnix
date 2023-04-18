#include "GLFWGraphicsContext.hpp"
#include "gl/gl_core_4_3.hpp"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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

GLFWGraphicsContext::ISize GLFWGraphicsContext::getFramebufferSize()
{
	ISize size;
	glfwGetFramebufferSize(_window, &size.width, &size.height);
	return size;
}

void GLFWGraphicsContext::resizeBuffer()
{
	auto size = getFramebufferSize();
	gl::Viewport(0, 0, size.width, size.height);
}

void GLFWGraphicsContext::clearBuffer(const glm::vec3& background)
{
	gl::ClearColor(background.r, background.g, background.b, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
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

void GLFWGraphicsContext::swapBuffers()
{
	glfwSwapBuffers(_window);
}

ILoaderImpl* GLFWGraphicsContext::createLoader()
{
	return new GLLoaderImpl();
}
