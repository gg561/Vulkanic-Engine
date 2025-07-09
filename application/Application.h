#pragma once

#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <string>
#include <window/Window.h>
#include <vulkan/vulkan.h>
#include <glm/vec3.hpp>
#include "Instance.h"
#include "Device.h"

using namespace glm;
using namespace window;

namespace application {
	class Application
	{
	private:
		std::function<void(void)> func;
		Window window;
		std::string name;
		vec3 applicationVersion;
		vec3 engineVersion;
		Instance instance;
		Device device;
	public:
		Application(Window window, vec3 applicationVersion, vec3 engineVersion);
		Application(unsigned int width, unsigned int height, std::string name, vec3 applicationVersion, vec3 engineVersion);
		~Application();


		Application* initVulkan();
		Application* cleanUp();
		Application* run();
		Application* operation();
		Application* setFunc(std::function<void(void)> func) {
			this->func = func;
			return this;
		}
		std::function<void(void)> getFunc() {
			return func;
		}
	};
}

