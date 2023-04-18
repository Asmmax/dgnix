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
	class StopWrapper
	{
	private:
		Application* _instance;

	public:
		StopWrapper();
		~StopWrapper();

		StopWrapper(const StopWrapper& other) = delete;
		void operator=(const StopWrapper& other) = delete;

		Application* get();
		void stop();
	};

private:
	Application();
	void initGraphics();
	void clear();

public:
	~Application();
	Application(const Application& other) = delete;
	void operator=(const Application& other) = delete;
	static Application& getInstance();
#ifdef _DEBUG
	static StopWrapper& getStopWrapper();
#endif // _DEBUG

	template<typename Impl>
	void bindImpl();

	Window* getWindow(int width, int height, const std::string& title);
};
