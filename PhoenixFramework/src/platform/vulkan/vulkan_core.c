#include <phoenix/platform/vulkan/context.h>

void phnx_create_instance(const cstr_t appName) {
	VkApplicationInfo appInfo = {0};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.pEngineName = "Phoenix";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
	appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);

	VkInstanceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.enabledExtensionCount = 0;
	createInfo.enabledLayerCount = 0;
	createInfo.pApplicationInfo = &appInfo;

	if (vkCreateInstance(&createInfo, NULL, &gInstance) != VK_SUCCESS) {
		PHNX_FATAL("Failed To Create Instance");
	}
}