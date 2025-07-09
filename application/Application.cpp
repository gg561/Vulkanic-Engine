#include "Application.h"
#include <set>
#include <cstring>

namespace application {

	Application::Application(Window window, vec3 applicationVersion, vec3 engineVersion) : window{ window }, name{ window.getName() }, instance{ name, applicationVersion, engineVersion }
	{

	}

	Application::Application(unsigned int width, unsigned int height, std::string name, vec3 applicationVersion, vec3 engineVersion) : window{ Window(width, height, name) }, name{ name }, applicationVersion{ applicationVersion }, instance{ name, applicationVersion, engineVersion }
	{
		
	}

	Application::~Application() {

	}

	Application* Application::initVulkan() 
	{
		instance.create();
		return this;
	}

	Application* Application::run()
	{ 
		try { 
			return operation(); 
		} catch (const std::exception& e) 
		{ 
			std::cerr << e.what() << '\n'; return nullptr; 
		} 
	}

	Application* Application::operation() {
		window
			.hint()
			->create();
		while (!window.shouldClose()) {
			if(func != nullptr)
				func();
			window.update();
		}
		return cleanUp();
	}

	Application* Application::cleanUp() 
	{
		instance.destroy();
		window.~Window();
		return this;
	}
}
