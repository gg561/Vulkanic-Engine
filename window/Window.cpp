#include "Window.h"
namespace window {
	Window::Window(unsigned int width, unsigned int height, std::string name) : width{ width }, height{ height }, name{ name.append(" Window")} {
		glfwInit();
	}

	Window::~Window() {
		glfwDestroyWindow(address);
		glfwTerminate();
	}

	Window* Window::hint() {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		return this;
	}

	Window* Window::create() {
		address = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		return this;
	}

	Window* Window::update() {
		glfwSwapBuffers(address);
		glfwPollEvents();
		return this;
	}

}