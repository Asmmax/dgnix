#pragma once
#include <memory>
#include <string>

class Window;
class IApplicationImpl;

class Application
{
private:
	bool _isValid;
	std::unique_ptr<IApplicationImpl> _impl;
	std::unique_ptr<Window> _window;

public:
#ifdef _DEBUG
	class StopWrapper
	{
	private:
		Application* _instance;

	public:
		StopWrapper();
		~StopWrapper();

		StopWrapper(const StopWrapper& other) = delete;
		StopWrapper& operator=(const StopWrapper& other) = delete;
		StopWrapper(StopWrapper&& other) = delete;
		StopWrapper& operator=(StopWrapper&& other) = delete;

		Application* get() const;
		void stop();
	};
#endif // _DEBUG

private:
	Application();
	void initGraphics();
	void clear();

public:
	~Application();
	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;
	Application(Application&& other) = delete;
	Application& operator=(Application&& other) = delete;
	static Application& getInstance();
#ifdef _DEBUG
	static StopWrapper& getStopWrapper();
#endif // _DEBUG

	template<typename Impl>
	void bindImpl();

	Window* getWindow(int width, int height, const std::string& title);
	double GetTime() const;
};

template<typename Impl>
void Application::bindImpl()
{
	clear();
	_impl = std::make_unique<Impl>();
	initGraphics();
}
