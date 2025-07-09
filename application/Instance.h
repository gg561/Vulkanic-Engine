#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <glm/vec3.hpp>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif




using namespace glm;
namespace application {
	class Instance
	{

	protected:
		std::string name;
		vec3 applicationVersion;
		vec3 engineVersion;
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		virtual std::vector<VkExtensionProperties> getSupportedExtensionProperties();
		virtual std::vector<const char*> getRequiredExtensionNames();
		virtual VkDebugUtilsMessengerEXT setupDebugCallback();
		virtual VkDebugUtilsMessengerCreateInfoEXT populateDebugCallback();

	public:
		Instance(std::string name, vec3 applicationVersion, vec3 engineVersion);
		virtual VkInstance create();
		virtual Instance* destroy();
		bool isValidationLayerSupported();

	};
}

