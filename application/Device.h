#pragma once
#include <vulkan/vulkan.h>
#include "Instance.h"
#include <optional>
namespace application{
	class Device
	{

	private:
		VkPhysicalDevice device = VK_NULL_HANDLE;
		uint32_t deviceCount = 0;
		Instance* instance;
		std::vector<VkPhysicalDevice> *devices;
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;

			bool isComplete()
			{
				return graphicsFamily.has_value();
			}
		};
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	public:
		Device(Instance* instance);
		~Device();
		Device* create();
		bool isDeviceSuitable(VkPhysicalDevice device);
		int rateDeviceSuitability(VkPhysicalDevice device);
		VkPhysicalDevice findMostSuitableDevice();
	};
}

