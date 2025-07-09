#include "Device.h"
#include <map>

namespace application {

	std::tuple<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> findDevicePropertiesAndFeatures(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceProperties(device, &properties);
		vkGetPhysicalDeviceFeatures(device, &features);
		return std::make_tuple(properties, features);
	}

	Device::Device(Instance* instance) : instance { instance }
	{

	}

	Device* Device::create()
	{
		vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw std::runtime_error("Failed to locate GPUs with Vulkan support!");
		}
		devices = new std::vector<VkPhysicalDevice>(deviceCount);
		vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, devices->data());
		VkPhysicalDevice physicalDevice = findMostSuitableDevice();
		return this;
	}

	bool Device::isDeviceSuitable(VkPhysicalDevice device)
	{
		std::tuple<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> tuple = findDevicePropertiesAndFeatures(device);
		VkPhysicalDeviceProperties properties = get<0>(tuple);
		VkPhysicalDeviceFeatures features = get<1>(tuple);
		QueueFamilyIndices queueFamily = findQueueFamilies(device);
		return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader && queueFamily.isComplete();
	}

	int Device::rateDeviceSuitability(VkPhysicalDevice device)
	{
		std::tuple<VkPhysicalDeviceProperties, VkPhysicalDeviceFeatures> tuple = findDevicePropertiesAndFeatures(device);
		VkPhysicalDeviceProperties properties = get<0>(tuple);
		VkPhysicalDeviceFeatures features = get<1>(tuple);
		QueueFamilyIndices queueFamily = findQueueFamilies(device);
		int score = 0;
		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}
		score += properties.limits.maxImageDimension2D;
		if (!features.geometryShader || !queueFamily.isComplete()) {
			return 0;
		}
		return score;
	}

	VkPhysicalDevice Device::findMostSuitableDevice()
	{
		std::multimap<int, VkPhysicalDevice> candidates;
		for (const auto& device : *devices) {
			int score = rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}
		if (candidates.rbegin()->first > 0) {
			return candidates.rbegin()->second;
		}
		else {
			throw std::runtime_error("Failed to locate suitable GPU!");
		}
	}

	Device::QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			i++;
		}
		return indices;
	}

	Device::~Device()
	{
		delete devices;
	}

}