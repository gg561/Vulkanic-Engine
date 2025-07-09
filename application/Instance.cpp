#include "Instance.h"
#include <set>
#include <GLFW/glfw3.h>
namespace application {


	VkResult createDebugCallback(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void destroyDebugCallback(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	Instance::Instance(std::string name, vec3 applicationVersion, vec3 engineVersion) : name{ name }, applicationVersion{ applicationVersion }, engineVersion{ engineVersion } {

	}

	VkInstance Instance::create()
	{
		if (enableValidationLayers && !isValidationLayerSupported()) {
			throw std::runtime_error("Validation Layers requested but not supported!");
		}
		//APP-INFO
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = name.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(applicationVersion.x, applicationVersion.y, applicationVersion.z);
		appInfo.pEngineName = name.append(" Engine").c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(engineVersion.x, engineVersion.y, engineVersion.z);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		//CREATE-INFO

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		std::vector<const char*> requiredExtensions = getRequiredExtensionNames();

		createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = populateDebugCallback();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
	}

	bool Instance::isValidationLayerSupported()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		for (const char* layerName : validationLayers) {
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}
			if (!layerFound) {
				return false;
			}
		}
		return true;
	}

	std::vector<VkExtensionProperties> Instance::getSupportedExtensionProperties()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		return extensions;
	}

	std::vector<const char*> Instance::getRequiredExtensionNames() 
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		//questionable part, could require recode
		std::vector<const char*> requiredExtensions;
		for (uint32_t i = 0; i < glfwExtensionCount; i++) {
			requiredExtensions.emplace_back(glfwExtensions[i]);
		}
		requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		if (enableValidationLayers) {
			requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		std::vector<VkExtensionProperties> supportedExtensions = getSupportedExtensionProperties();
		for (const char* requiredExtension : requiredExtensions) {
			bool supported = false;
			for (VkExtensionProperties supportedExtension : supportedExtensions) {
				if (strcmp(requiredExtension, supportedExtension.extensionName) == 0) {
					supported = true;
					break;
				}
			}
			if (!supported) {
				throw std::runtime_error("Unsupported Extensions requested!");
			}
		}
		return requiredExtensions;
	}

	Instance* Instance::destroy() 
	{
		if (enableValidationLayers) {
			destroyDebugCallback(instance, debugMessenger, nullptr);
		}
		vkDestroyInstance(instance, nullptr);
		return this;
	}

	VkDebugUtilsMessengerEXT Instance::setupDebugCallback() 
	{
		if (!enableValidationLayers) return nullptr;
		VkDebugUtilsMessengerCreateInfoEXT createInfo = populateDebugCallback();

		if (createDebugCallback(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("Failed to setup debug messenger!");
		}
		return debugMessenger;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	VkDebugUtilsMessengerCreateInfoEXT Instance::populateDebugCallback() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
		return createInfo;
	}
}