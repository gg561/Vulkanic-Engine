#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <application/Application.h>
using namespace application;

int main() {
	Application app{ 800, 600, "Vulkanic", vec3(1, 0, 0), vec3(1, 0, 0)};
	app.initVulkan()->setFunc(nullptr)->run();
	return 0;

}