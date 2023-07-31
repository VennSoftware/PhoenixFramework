#include <phoenix/platform/vulkan/context.h>
#include <phoenix/phoenix.h>

//Forward Decls
bool pxIsDeviceSuitable(VkDevice);
void pxSetupIndices();
const cstr_t pxGetVendorName(VkVendorId);

void pxInitializeVulkan(const cstr_t appName) {
	
	pxCreateInstance(appName);
	pxSetupPhysicalDevice();
	pxSetupIndices();
}


void pxCreateInstance(const cstr_t appName) {
	VkApplicationInfo appInfo = {0};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.pEngineName = PHNX_NAME;
	appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
	appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.enabledExtensionCount = 0;
	createInfo.enabledLayerCount = 0;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	if (vkCreateInstance(&createInfo, NULL, &gInstance) != VK_SUCCESS) {
		PHNX_FATAL("Failed To Create Instance");
	}

	PHNX_LOG("Created Instance");
}
void pxSetupPhysicalDevice() {
	U32 deviceCount = 0;
	vkEnumeratePhysicalDevices(gInstance, &deviceCount, NULL);

	if (deviceCount == 0) { PHNX_FATAL("Failed To Find Vulkan Compatible Device."); }

	gPhysicalDevices = pxMalloc(deviceCount * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(gInstance, &deviceCount, gPhysicalDevices);

	PHNX_LOG("Found %d Device(s):", deviceCount);

	for (int i = 0; i < deviceCount; i++) {
		VkPhysicalDeviceProperties props = {0};
		vkGetPhysicalDeviceProperties(gPhysicalDevices[i], &props);
		PHNX_LOG(" - Device: %s (Vendor: 0x%X)", props.deviceName, props.vendorID);
		pxGetIndices(gPhysicalDevices[i]);
	}

	for (int i = 0; i < deviceCount; i++) {
		if (pxIsDeviceSuitable(gPhysicalDevices[i])) {
			gPhysicalDevice = gPhysicalDevices[i];
			break;
		}
	}

	if (gPhysicalDevice == VK_NULL_HANDLE) {
		PHNX_FATAL("Failed To Select Suitable Device");
	}
	else {
		VkPhysicalDeviceProperties props = { 0 };
		vkGetPhysicalDeviceProperties(gPhysicalDevice, &props);
		PHNX_LOG("Selected Device: %s", props.deviceName);
	}



}

void pxDestroyContext() {
	PHNX_LOG("Shutting Down Vulkan");
	pxFree(gPhysicalDevices);
	vkDestroyInstance(gInstance, NULL);
}


bool pxIsDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	QueueFamilyIndices indices = pxGetIndices(device);
	return (indices.graphicsFamily >= 0);
}

bool pxIndicesAreComplete() {
	return gIndices.graphicsFamily >= 0;
}

void pxSetupIndices() {
	U32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gPhysicalDevice, &queueFamilyCount, NULL);
	PHNX_LOG("Queue Family Count: %d", queueFamilyCount); 

	VkQueueFamilyProperties* pProps = pxCalloc(queueFamilyCount, sizeof(VkQueueFamilyProperties));
	vkGetPhysicalDeviceQueueFamilyProperties(gPhysicalDevice, &queueFamilyCount, pProps);
	for (int i = 0; i < queueFamilyCount; i++) {
		VkQueueFamilyProperties family = pProps[i];
		PHNX_LOG("Queue: %d - Flags: %c%c%c%c", i,
			(family.queueFlags & VK_QUEUE_GRAPHICS_BIT) ? 'G' : '-',
			(family.queueFlags & VK_QUEUE_COMPUTE_BIT) ? 'C' : '-',
			(family.queueFlags & VK_QUEUE_PROTECTED_BIT) ? 'P' : '-',
			(family.queueFlags & VK_QUEUE_TRANSFER_BIT) ? 'T' : '-'
		);
	}

	for (int i = 0; i < queueFamilyCount; i++) {
		VkQueueFamilyProperties family = pProps[i];
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) { 
			gIndices.graphicsFamily = i;
		}

		if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			gIndices.computeFamily = i;
		}

		if (gIndices.computeFamily > -1 && gIndices.computeFamily > -1) break;

	}

	PHNX_LOG("Indices:\n\tGraphics: %d\n\tCompute: %d", gIndices.graphicsFamily, gIndices.computeFamily);

	PHNX_ASSERT(gIndices.graphicsFamily >= 0);

	pxFree(pProps);
}


QueueFamilyIndices pxGetIndices(VkPhysicalDevice device) {
	U32 queueFamilyCount = 0;
	QueueFamilyIndices indices = { .graphicsFamily = -1, .computeFamily = -1, .presentFamily = -1 };
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties* pProps = pxCalloc(queueFamilyCount, sizeof(VkQueueFamilyProperties));
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, pProps);

	for (int i = 0; i < queueFamilyCount; i++) {
		VkQueueFamilyProperties family = pProps[i];
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			indices.computeFamily = i;
		}

	}

	pxFree(pProps);

	return indices;
}