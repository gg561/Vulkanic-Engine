#pragma once
#include <GLFW/glfw3.h>
#include <string>
namespace window {
	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, std::string name);
		~Window();
		Window* create();
		Window* hint();
		Window* update();
		bool shouldClose() { return glfwWindowShouldClose(address); };
		std::string getName() { return name; }

	private:
		unsigned int width;
		unsigned int height;
		std::string name;

		GLFWwindow* address;
	};
}
