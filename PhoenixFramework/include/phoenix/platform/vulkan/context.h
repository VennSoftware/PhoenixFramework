#ifndef PHNX_VK_CONTEXT_H
#define PHNX_VK_CONTEXT_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <phoenix/common.h>

typedef struct QueueFamilyIndices {
	I32 graphicsFamily;
	I32 presentFamily;
	I32 computeFamily;
} QueueFamilyIndices;

#define REQUIRED_EXT_COUNT (1)

static const cstr_t requiredExtensions[REQUIRED_EXT_COUNT] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


static VkInstance gInstance;
static VkPhysicalDevice gPhysicalDevice = VK_NULL_HANDLE;
static VkPhysicalDevice* gPhysicalDevices;
static QueueFamilyIndices gIndices = { .graphicsFamily = -1, .computeFamily = -1, .presentFamily = -1 };
static VkSurfaceKHR gSurface;
static VkDevice gDevice;

static VkQueue gGraphicsQueue = VK_NULL_HANDLE;
static VkQueue gPresentQueue = VK_NULL_HANDLE;

static const VkPhysicalDeviceType REQUIRED_DEVICE_TYPE = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

QueueFamilyIndices pxGetIndices(VkPhysicalDevice device);

void pxInitializeVulkan(const cstr_t appName);

void pxCreateInstance(const cstr_t appName);
void pxCreateSurfaceGLFW();
void pxSetupPhysicalDevice();
void pxSetupLogicalDevice();



void pxDestroyContext();


#endif // !PHNX_VK_CONTEXT_H
