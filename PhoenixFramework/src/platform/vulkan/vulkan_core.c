#include <phoenix/platform/vulkan/context.h>
#include <phoenix/phoenix.h>
#include <phoenix/window.h>

//Forward Decls
bool pxIsDeviceSuitable(VkDevice);
void pxSetupIndices();
const cstr_t pxGetVendorName(VkVendorId);
bool pxIndicesAreComplete();
bool pxCheckDeviceExtensionSupport(VkPhysicalDevice device);

void pxInitializeVulkan(const cstr_t appName) {
	
	pxCreateInstance(appName);
	pxCreateSurfaceGLFW();
	pxSetupPhysicalDevice();
	pxSetupIndices();
	PHNX_ASSERT(pxIndicesAreComplete());
	pxSetupLogicalDevice();
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
void pxCreateSurfaceGLFW() {
	if (glfwCreateWindowSurface(gInstance, pxGetWindow(), NULL, &gSurface) != VK_SUCCESS) {
		PHNX_FATAL("Failed To Create Surface");
	}
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
	vkDestroySurfaceKHR(gInstance, gSurface, NULL);
	vkDestroyInstance(gInstance, NULL);
}


bool pxIsDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	QueueFamilyIndices indices = pxGetIndices(device);

	bool extensionsPresent = pxCheckDeviceExtensionSupport(device);

	return (indices.graphicsFamily >= 0 && indices.presentFamily >= 0) && extensionsPresent;
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
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(gPhysicalDevice, i, gSurface, &presentSupport);
		PHNX_LOG("Queue: %d - Flags: %c%c%c%c%c", i,
			(family.queueFlags & VK_QUEUE_GRAPHICS_BIT) ? 'G' : '-',
			(family.queueFlags & VK_QUEUE_COMPUTE_BIT) ? 'C' : '-',
			(family.queueFlags & VK_QUEUE_PROTECTED_BIT) ? 'R' : '-',
			(family.queueFlags & VK_QUEUE_TRANSFER_BIT) ? 'T' : '-',
			(presentSupport ? 'P' : '-')
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

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(gPhysicalDevice, i, gSurface, &presentSupport);

		if (presentSupport) {
			gIndices.presentFamily = i;
		}

		if (gIndices.graphicsFamily > -1 && gIndices.computeFamily > -1 && gIndices.presentFamily > -1) break;

	}

	PHNX_LOG("Indices:\n\tGraphics: %d\n\tCompute: %d\n\tPresent: %d", gIndices.graphicsFamily, gIndices.computeFamily, gIndices.presentFamily);

	

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

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, gSurface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.graphicsFamily > -1 && indices.computeFamily > -1 && indices.presentFamily > -1) break;

	}

	pxFree(pProps);

	return indices;
}


void pxSetupLogicalDevice() {
	VkPhysicalDeviceFeatures deviceFeatures = {0};
	I32 families[] = { gIndices.graphicsFamily, gIndices.presentFamily };
	VkDeviceQueueCreateInfo queueInfos[2] = { 0, 0 };
	float queuePriority = 1.0f;
	for (int i = 0; i < 2; i++) {
		queueInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfos[i].queueFamilyIndex = gIndices.graphicsFamily;
		queueInfos[i].queueCount = 1;
		
		queueInfos[i].pQueuePriorities = &queuePriority;
	}

	

	VkDeviceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueInfos;
	createInfo.queueCreateInfoCount = 2;

	createInfo.pEnabledFeatures = &deviceFeatures;
	if (vkCreateDevice(gPhysicalDevice, &createInfo, NULL, &gDevice) != VK_SUCCESS) {
		PHNX_FATAL("Failed To Create Logical Device");
	}

	PHNX_LOG("Created Logical Device");

	vkGetDeviceQueue(gDevice, gIndices.graphicsFamily, 0, &gGraphicsQueue);
	vkGetDeviceQueue(gDevice, gIndices.presentFamily, 0, &gPresentQueue);
}

bool pxCheckDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

	VkExtensionProperties* pExtensions = pxCalloc(extensionCount, sizeof(VkExtensionProperties));
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, pExtensions);
	bool passed = true;
	for (int i = 0; i < REQUIRED_EXT_COUNT; i++) {
		bool found = false;
		for (int j = 0; j < extensionCount; j++) {
			if (strcmp(pExtensions[j].extensionName, requiredExtensions[i]) == 0) {
				found = true;
				break;
			}
		}
		if (!found) { passed = false; break; }
	}

	end_check:

	pxFree(pExtensions);

	return passed;
}